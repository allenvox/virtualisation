#include <iostream>
#include <pqxx/pqxx>
#include "httplib.h"
#include "json.hpp"

using json = nlohmann::json;

const char* db_host = std::getenv("POSTGRES_HOST");
const char* db_user = std::getenv("POSTGRES_USER");
const char* db_password = std::getenv("POSTGRES_PASSWORD");
const char* db_name = std::getenv("POSTGRES_DB");

std::string conn_string = "host=" + std::string(db_host) +
                          " user=" + std::string(db_user) +
                          " password=" + std::string(db_password) +
                          " dbname=" + std::string(db_name);

void init_db() {
    try {
        pqxx::connection conn(conn_string);
        pqxx::work txn(conn);
        txn.exec(R"(
            CREATE TABLE IF NOT EXISTS users (
                id SERIAL PRIMARY KEY,
                name VARCHAR(100) NOT NULL,
                email VARCHAR(100) UNIQUE NOT NULL
            );
        )");
        txn.commit();
        std::cout << "Database initialized.\n";
    } catch (const std::exception &e) {
        std::cerr << "Database error: " << e.what() << '\n';
    }
}

int main() {
    init_db();
    
    httplib::Server svr;

    // GET /users - Получить всех пользователей
    svr.Get("/users", [](const httplib::Request&, httplib::Response& res) {
        pqxx::connection conn(conn_string);
        pqxx::work txn(conn);
        pqxx::result users = txn.exec("SELECT id, name, email FROM users");

        json response = json::array();
        for (const auto& row : users) {
            response.push_back({{"id", row[0].as<int>()}, {"name", row[1].c_str()}, {"email", row[2].c_str()}});
        }

        res.set_content(response.dump(), "application/json");
    });

    // PUT /users - Создать нового пользователя
    svr.Put("/users", [](const httplib::Request& req, httplib::Response& res) {
        try {
            json data = json::parse(req.body);
            std::string name = data["name"];
            std::string email = data["email"];

            pqxx::connection conn(conn_string);
            pqxx::work txn(conn);
            txn.exec_params("INSERT INTO users (name, email) VALUES ($1, $2)", name, email);
            txn.commit();

            res.set_content(R"({"status":"user added"})", "application/json");
        } catch (const std::exception &e) {
            res.status = 400;
            res.set_content(R"({"error":")" + std::string(e.what()) + R"("})", "application/json");
        }
    });

    // GET /users/{id} - Получить пользователя по ID
    svr.Get(R"(/users/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        int user_id = std::stoi(req.matches[1]);
        pqxx::connection conn(conn_string);
        pqxx::work txn(conn);
        pqxx::result user = txn.exec_params("SELECT id, name, email FROM users WHERE id = $1", user_id);

        if (user.empty()) {
            res.status = 404;
            res.set_content(R"({"error":"user not found"})", "application/json");
        } else {
            json response = {{"id", user[0][0].as<int>()}, {"name", user[0][1].c_str()}, {"email", user[0][2].c_str()}};
            res.set_content(response.dump(), "application/json");
        }
    });

    // DELETE /users/{id} - Удалить пользователя
    svr.Delete(R"(/users/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        int user_id = std::stoi(req.matches[1]);
        pqxx::connection conn(conn_string);
        pqxx::work txn(conn);
        txn.exec_params("DELETE FROM users WHERE id = $1", user_id);
        txn.commit();

        res.set_content(R"({"status":"user deleted"})", "application/json");
    });

    std::cout << "Server is running on http://localhost:8000\n";
    svr.listen("0.0.0.0", 8000);
}
