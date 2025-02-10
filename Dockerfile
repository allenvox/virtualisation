FROM gcc:12 AS build
RUN apt-get update && apt-get install -y cmake
COPY . /app
WORKDIR /app
RUN g++ -o my-app app.cpp -std=c++17 -static -lpthread

FROM alpine:latest
RUN apk add --no-cache libstdc++
COPY --from=build /app/my-app /usr/local/bin/my-app
EXPOSE 8000
CMD ["my-app"]
