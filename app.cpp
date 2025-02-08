#include <iostream>
#include <random>
#include "httplib.h"

int generate_random_number() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    return dis(gen);
}

int main() {
    httplib::Server svr;
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        int random_number = generate_random_number();
        res.set_content("Random number: " + std::to_string(random_number), "text/plain");
    });
    std::cout << "Server is running on http://localhost:8000\n";
    svr.listen("0.0.0.0", 8000);
    return 0;
}
