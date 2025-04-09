# Chess3001

![GitHub repo size](https://img.shields.io/github/repo-size/pedrol3001/Chess3001?style=for-the-badge)
![GitHub language count](https://img.shields.io/github/languages/count/pedrol3001/Chess3001?style=for-the-badge)
![GitHub stars](https://img.shields.io/github/stars/pedrol3001/Chess3001?style=for-the-badge&color=00aa00)
![GitHub forks](https://img.shields.io/github/forks/pedrol3001/Chess3001?style=for-the-badge&color=aa00aa)
![Bitbucket open issues](https://img.shields.io/bitbucket/issues/pedrol3001/Chess3001?style=for-the-badge)
![Bitbucket open pull requests](https://img.shields.io/bitbucket/pr-raw/pedrol3001/Chess3001?style=for-the-badge)

> Uci Chess Engine developed in c++ using [chess programming principles](https://www.chessprogramming.org) and some optimizations used in [Stockfish](https://github.com/official-stockfish/Stockfish.git)

### Improvements

- [X] Bitboards as borad representation
- [X] Magic Bitboards to generate sliding pieces moves
- [X] Principal Variation Search to find best move
- [X] Quiescence Search to optimize the search algorithm
- [X] Move ordering to optimize alpha beta pruning
- [X] Linear static evaluation of pieces
- [X] Uci protocol to communicate with a GUI
- [ ] Improve move generation time
- [ ] Finish Uci commands
- [ ] C++ Algorithms optimizations

## 💻 Dependencies

Before start verify this dependencies.

* You should install the lattest version of `g++` compiler and `make`.
* Should have a `Windows / Linux / Mac`.
* Must understand the basics of chess programming `https://www.chessprogramming.org`.

## ☕ Using Chess3001

To use Chess3001, follow this instructions:

### Build

```
cd src
make build
./chess3001
```

Use `make help` to see all possible build options.

### Run

* Download a chess uci gui interface. Example, [Arena Chess](http://www.playwitharena.de)

## 📫 Contributing with Chess3001

Para contribuir com <nome_do_projeto>, siga estas etapas:

1. Fork this repository
2. Create a branch : `git checkout -b branch_name`.
3. Make the desired changes: `git commit -m 'commit_message'`
4. Envie para o branch original: `git push origin branch_name`
5. Create the pull request.

See the github documentation about [how to create a pull request](https://help.github.com/en/github/collaborating-with-issues-and-pull-requests/creating-a-pull-request).

## 🤝 Collaborators

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

# Chess3001 REST API with Ruby Sinatra

A simple REST API wrapper for the Chess3001 engine using Ruby and Sinatra.

## Prerequisites

- Ruby 2.6+
- Sinatra gem
- Compiled Chess3001 engine

## Installation

1. Make sure your Chess3001 engine is compiled:
   ```
   cd src
   make
   ```

2. Install the required Ruby gems:
   ```
   gem install sinatra json
   ```

## Running the API

Start the API server:

```
ruby chess_api.rb
```

The server will run on `http://localhost:4567`.

## API Endpoints

### Get Board State

```
GET /board
```

Returns the current board state in FEN notation and a text representation.

### Make a Move

```
POST /move
```

Request body:
```json
{
  "move": "e2e4"
}
```

Makes the specified player move and then an engine move. Returns both moves and the resulting board state.

### Reset Game

```
POST /reset
```

Resets the game to the starting position.

## Example Usage

Using curl:

```bash
# Get the current board state
curl http://localhost:4567/board

# Make a move
curl -X POST http://localhost:4567/move \
  -H "Content-Type: application/json" \
  -d '{"move": "e2e4"}'

# Reset the game
curl -X POST http://localhost:4567/reset
```

Using Ruby:

```ruby
require 'net/http'
require 'uri'
require 'json'

# Get the current board state
uri = URI.parse('http://localhost:4567/board')
response = Net::HTTP.get_response(uri)
puts JSON.parse(response.body)

# Make a move
uri = URI.parse('http://localhost:4567/move')
http = Net::HTTP.new(uri.host, uri.port)
request = Net::HTTP::Post.new(uri.request_uri, {'Content-Type' => 'application/json'})
request.body = {move: 'e2e4'}.to_json
response = http.request(request)
puts JSON.parse(response.body)

# Reset the game
uri = URI.parse('http://localhost:4567/reset')
http = Net::HTTP.new(uri.host, uri.port)
request = Net::HTTP::Post.new(uri.request_uri)
response = http.request(request)
puts JSON.parse(response.body)
```


## 📝 License

This project is under a licese. See the file [License](LICENSE) to see more details.

[⬆ Get to the top](#Chess3001)<br>
