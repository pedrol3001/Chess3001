# Chess3001

![GitHub repo size](https://img.shields.io/github/repo-size/pedrol3001/Chess3001?style=for-the-badge)
![GitHub language count](https://img.shields.io/github/languages/count/pedrol3001/Chess3001?style=for-the-badge)
![GitHub stars](https://img.shields.io/github/stars/pedrol3001/Chess3001?style=for-the-badge&color=00aa00)
![GitHub forks](https://img.shields.io/github/forks/pedrol3001/Chess3001?style=for-the-badge&color=aa00aa)
![Bitbucket open issues](https://img.shields.io/bitbucket/issues/pedrol3001/Chess3001?style=for-the-badge)
![Bitbucket open pull requests](https://img.shields.io/bitbucket/pr-raw/pedrol3001/Chess3001?style=for-the-badge)

> Chess3001 is a full-stack chess application consisting of a powerful UCI chess engine written in C++, a Ruby Sinatra REST API, and a React frontend. The engine uses advanced [chess programming principles](https://www.chessprogramming.org) and optimizations inspired by [Stockfish](https://github.com/official-stockfish/Stockfish.git).

## 🏗️ Project Structure

The project is organized into three main components:

- **Engine**: A high-performance C++ chess engine with UCI protocol support
- **Backend**: A Ruby Sinatra REST API that interfaces with the engine
- **Frontend**: A React-based web interface for playing against the engine

## 🚀 Chess Engine

The core of Chess3001 is a powerful chess engine written in C++ that implements various advanced chess programming techniques.

### Features

- [x] Bitboards for efficient board representation
- [x] Magic Bitboards for fast sliding piece move generation
- [x] Principal Variation Search algorithm for finding the best moves
- [x] Quiescence Search to optimize the search algorithm
- [x] Move ordering to enhance alpha-beta pruning efficiency
- [x] Linear static evaluation of pieces
- [x] UCI protocol for GUI communication
- [ ] Improved move generation time
- [ ] Complete UCI command implementation
- [ ] Additional C++ algorithm optimizations

### Building the Engine

```bash
# Navigate to the engine directory
cd engine

# Build the engine
make build

# Run the engine directly (UCI mode)
./chess3001
```

Use `make help` to see all available build options.

### Using with Chess GUIs

If you prefer to use the engine with a standard chess GUI:

1. Download a UCI-compatible chess GUI (e.g., [Arena Chess](http://www.playwitharena.de))
2. Configure the GUI to use the `chess3001` executable
3. Start a new game with the engine

## 🌐 Backend API

The Chess3001 backend is a Ruby Sinatra REST API that provides an interface between the frontend and the chess engine.

### Features

- Simple JSON-based API for interacting with the chess engine
- Endpoints for retrieving board state, making moves, and resetting games
- Cross-origin resource sharing (CORS) support for frontend integration
- Custom command handling for communicating with the UCI engine

### Setup

```bash
# Navigate to the backend directory
cd backend

# Install required gems
gem install sinatra json sinatra-cross-origin

# Start the API server
ruby chess_api.rb
```

The server will run on `http://localhost:4567`.

### API Endpoints

#### Get Board State

```
GET /api/board
```

Returns the current board state in FEN notation.

#### Make a Move

```
POST /api/move
```

Request body:
```json
{
  "move": "e2e4"
}
```

Makes the specified player move and returns the updated board state.

#### Reset Game

```
POST /api/reset
```

Resets the game to the starting position.

### Implementation Notes

- The API uses the `fen` command (instead of `d`) to retrieve the FEN string representation of the board
- The engine returns the raw FEN string without any prefix
- All endpoints use the `/api/` prefix for consistency and clarity

## 💻 Frontend

The Chess3001 frontend is a modern React application that provides a user-friendly interface for playing against the chess engine.

### Features

- Modern React-based user interface
- Interactive chessboard with drag-and-drop move capability
- Real-time game state synchronization with the backend
- Responsive design for desktop and mobile play

### Setup

```bash
# Navigate to the frontend directory
cd frontend

# Install dependencies
npm install

# Start the development server
npm start
```

The frontend will be available at `http://localhost:3000`.

### Implementation Notes

- Uses the native fetch API with explicit CORS mode and headers
- Connects directly to the backend at http://localhost:4567
- Includes detailed error handling for connection issues

## 📋 Prerequisites

Before getting started, ensure you have:

- **For the Engine**:
  - Latest version of `g++` compiler
  - `make` build system
  - Windows, Linux, or macOS

- **For the Backend**:
  - Ruby 2.6+
  - Sinatra gem
  - JSON gem
  - sinatra-cross-origin gem

- **For the Frontend**:
  - Node.js and npm
  - React

## 🤝 Contributing

To contribute to Chess3001:

1. Fork this repository
2. Create a branch: `git checkout -b feature_branch_name`
3. Make your changes and commit them: `git commit -m 'Add some feature'`
4. Push to the original branch: `git push origin feature_branch_name`
5. Create a pull request

See GitHub's documentation on [creating a pull request](https://help.github.com/en/github/collaborating-with-issues-and-pull-requests/creating-a-pull-request).

## 👥 Collaborators

<table>
  <tr>
    <td align="center">
      <a href="#">
        <img src="https://avatars.githubusercontent.com/u/33810395?s=400&u=5ad2c222c48cbba0e4ade2057eb8a599590d235a&v=4" width="100px;" alt="Pedro Luis Github Foto"/><br>
        <sub>
          <b>Pedro Luis</b>
        </sub>
      </a>
    </td>
  </tr>
</table>

## 📝 License

This project is licensed under the terms specified in the [LICENSE](LICENSE) file.

[⬆ Get to the top](#Chess3001)
