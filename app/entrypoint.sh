#!/bin/bash
set -e
until pg_isready -h "$POSTGRES_HOST" -U "$POSTGRES_USER"; do
  echo "Waiting for PostgreSQL..."
  sleep 2
done
echo "PostgreSQL is up - running app..."
./db-app
