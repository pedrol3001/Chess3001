require 'sinatra'
require 'json'
require 'open3'

# Configuration
set :bind, '0.0.0.0'
set :port, ENV['PORT'] ? ENV['PORT'].to_i : 4567

# Add CORS headers to all responses
before do
  headers['Access-Control-Allow-Origin'] = '*'
  headers['Access-Control-Allow-Methods'] = 'GET, POST, OPTIONS'
  headers['Access-Control-Allow-Headers'] = 'Content-Type, Accept'
  
  # Handle preflight OPTIONS requests
  if request.request_method == 'OPTIONS'
    halt 200
  end
end

# Helper methods for standardized responses
def success_response(data)
  {
    status: "success",
    data: data
  }.to_json
end

def error_response(message, status_code = 400)
  status status_code
  {
    status: "error",
    error: {
      message: message,
      code: status_code
    }
  }.to_json
end

# Path to the chess engine executable
ENGINE_PATH = ENV.fetch('ENGINE_PATH')
STATE_FILE = "./game_state.txt"

# Default starting position in FEN notation
DEFAULT_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

# Ensure the chess engine exists
def ensure_engine_exists
  unless File.exist?(ENGINE_PATH)
    halt 500, { error: "Chess engine not found at #{ENGINE_PATH}. Make sure to compile it first." }.to_json
  end
end

# Save the current game state to a file
def save_state(fen)
  File.write(STATE_FILE, fen)
end

# Load the current game state from a file
def load_state
  unless File.exist?(STATE_FILE)
    # If no state file exists, create one with the default position
    save_state(DEFAULT_POSITION)
    return DEFAULT_POSITION
  end
  
  File.read(STATE_FILE).strip
end

# Communicate with the chess engine
def communicate_with_engine(commands)
  ensure_engine_exists
  
  response = []
  
  puts "[ENGINE LOG] Sending commands to engine: #{commands.join(', ')}"
  
  IO.popen(ENGINE_PATH, "r+") do |io|
    commands.each do |cmd|
      puts "[ENGINE LOG] > #{cmd}"
      io.puts cmd
    end
    io.close_write
    
    while line = io.gets
      puts "[ENGINE LOG] < #{line.chomp}"
      response << line.chomp
    end
  end
  
  puts "[ENGINE LOG] Engine response: #{response.inspect}"
  
  response
end

# Extract the FEN from the engine response
def extract_fen_from_response(response)
  # Look for the line that starts with 'fen '
  fen_line = response.find { |line| line.start_with?("fen ") }
  
  # Return the FEN string without the 'fen ' prefix
  fen_line ? fen_line.sub("fen ", "").strip : nil
end

# Get the current board state
def get_board_state
  begin
    current_fen = load_state
    commands = [
      "isready",
      "position fen #{current_fen}",
      "fen"  # Display position to get FEN
    ]
    
    response = communicate_with_engine(commands)
    
    # Extract the FEN from the response
    fen = extract_fen_from_response(response) || current_fen
    
    { fen: fen }
  rescue => e
    puts "[ERROR] Failed to get board state: #{e.message}"
    puts e.backtrace.join("\n")
    raise "Failed to get board state: #{e.message}"
  end
end

# Make a player move and then an engine move
def make_move(move_str)
  begin
    current_fen = load_state
    
    # First, make the player's move
    commands = [
      "isready",
      "position fen #{current_fen} moves #{move_str}",
      "fen"  # Display position to get FEN
    ]
    
    response = communicate_with_engine(commands)
    
    # Extract the FEN after player's move
    player_fen = extract_fen_from_response(response)
    
    # Check if the move is valid by comparing FENs
    # If the FEN didn't change (except possibly for move counters), the move was invalid
    if !player_fen
      raise "Could not extract FEN after player move"
    end
    
    # Compare the relevant parts of the FEN (board position, castling rights, en passant)
    # Ignore move counters which might change even for invalid moves
    current_fen_parts = current_fen.split(' ')[0..3]
    player_fen_parts = player_fen.split(' ')[0..3]
    
    if current_fen_parts == player_fen_parts
      raise "Illegal move: #{move_str}"
    end
    
    # Now, make the engine's move
    commands = [
      "isready",
      "position fen #{player_fen}",
      "go depth 7"
    ]
    
    response = communicate_with_engine(commands)
    
    # Find the engine's move from the response
    engine_move = nil
    response.each do |line|
      if line.start_with?("bestmove")
        engine_move = line.split[1]
        break
      end
    end
    
    if !engine_move
      raise "Engine did not return a move"
    end
    
    # Apply the engine's move
    commands = [
      "isready",
      "position fen #{player_fen} moves #{engine_move}",
      "fen"  # Display position to get FEN
    ]
    
    response = communicate_with_engine(commands)
    
    # Extract the FEN after engine's move
    final_fen = extract_fen_from_response(response)
    if !final_fen
      raise "Could not extract FEN after engine move"
    end
    
    # Save the new state
    save_state(final_fen)
    
    {
      player_move: move_str,
      engine_move: engine_move,
      fen: final_fen
    }
  rescue => e
    puts "[ERROR] Failed to make move: #{e.message}"
    puts e.backtrace.join("\n") if e.backtrace
    raise e
  end
end

# Reset the game to the starting position
def reset_game
  begin
    # Default starting position
    default_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    save_state(default_fen)
    
    {
      message: "Game reset to starting position",
      fen: default_fen
    }
  rescue => e
    puts "[ERROR] Failed to reset game: #{e.message}"
    puts e.backtrace.join("\n")
    raise "Failed to reset game: #{e.message}"
  end
end

# API Routes

# Get the current board state
get '/api/board' do
  content_type :json
  begin
    data = get_board_state
    # Ensure we have a valid FEN string
    if data[:fen].nil? || data[:fen].empty?
      error_response("Invalid or missing FEN string")
    else
      success_response(data)
    end
  rescue => e
    error_response(e.message)
  end
end

# Make a move
post '/api/move' do
  content_type :json
  begin
    # Parse the move from the request body
    request_payload = JSON.parse(request.body.read)
    move_str = request_payload['move']
    
    if !move_str || move_str.empty?
      return error_response("Move parameter is required")
    end
    
    data = make_move(move_str)
    # Ensure we have a valid FEN string
    if data[:fen].nil? || data[:fen].empty?
      error_response("Invalid or missing FEN string after move")
    else
      success_response(data)
    end
  rescue JSON::ParserError
    error_response("Invalid JSON in request body")
  rescue => e
    # Determine if this is a client error (like illegal move) or server error
    if e.message.include?("Illegal move") || e.message.include?("Invalid move")
      error_response(e.message, 400)
    else
      error_response("Server error: #{e.message}", 500)
    end
  end
end

# Reset the game to the starting position
post '/api/reset' do
  content_type :json
  begin
    success_response(reset_game)
  rescue => e
    error_response(e.message, 500)
  end
end


