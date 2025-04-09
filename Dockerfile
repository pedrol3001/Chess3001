FROM ruby:3.2-slim as base

# Install dependencies for building the chess engine
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the engine source code
COPY engine /app/engine

# Build the chess engine
WORKDIR /app/engine
RUN make build ARCH=x86-64-modern

# Copy the backend API
WORKDIR /app
COPY backend /app/backend

# Install Ruby dependencies
WORKDIR /app/backend
RUN gem install sinatra sinatra-contrib json

# Set environment variables
ENV ENGINE_PATH="/app/engine/app"
ENV PORT=4567

# Expose the API port
EXPOSE 4567

# Start the API server
CMD ["ruby", "chess_api.rb"]
