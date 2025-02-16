FROM debian:latest AS builder
RUN apt-get update && apt-get install -y g++ cmake make libpqxx-dev libpq-dev wget git && rm -rf /var/lib/apt/lists/*
WORKDIR /usr/src/app
COPY . .
RUN g++ -o db-app app.cpp -lpqxx -lpq -pthread

FROM debian:latest
RUN apt-get update && apt-get install -y libpqxx-dev libpq-dev postgresql-client && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY --from=builder /usr/src/app/db-app /app/db-app
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
