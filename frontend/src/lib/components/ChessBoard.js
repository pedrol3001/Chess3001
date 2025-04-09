import React, { useState, useEffect, useCallback } from 'react';
import { Chessboard } from 'react-chessboard';
import './ChessBoard.css';

// Simple Snackbar component
const Snackbar = ({ message, type, onClose }) => {
  useEffect(() => {
    // Auto-close after 5 seconds
    const timer = setTimeout(() => {
      if (onClose) onClose();
    }, 5000);
    
    return () => clearTimeout(timer);
  }, [message, onClose]);
  
  if (!message) return null;
  
  return (
    <div className={`snackbar ${type || 'error'}`}>
      <div className="snackbar-content">
        <span>{message}</span>
        <button onClick={onClose}>×</button>
      </div>
    </div>
  );
};

// Helper function to convert FEN to board object
const fenToObj = (fen) => {
  const boardObj = {};
  if (!fen) return boardObj;
  
  // Get the board part of the FEN (before the first space)
  const boardPart = fen.split(' ')[0];
  const rows = boardPart.split('/');
  
  // Loop through each row
  for (let i = 0; i < 8; i++) {
    let row = rows[i];
    let fileIndex = 0;
    
    // Process each character in the row
    for (let j = 0; j < row.length; j++) {
      const char = row[j];
      
      // If it's a number, skip that many files
      if (!isNaN(char)) {
        fileIndex += parseInt(char, 10);
      } else {
        // It's a piece - add it to the board object
        const file = String.fromCharCode(97 + fileIndex); // 'a' through 'h'
        const rank = 8 - i; // 8 down to 1
        const square = `${file}${rank}`;
        boardObj[square] = char;
        fileIndex++;
      }
    }
  }
  
  return boardObj;
};

/**
 * Simple Chess Board Component
 * @param {Object} props - Component props
 * @param {Object} props.api - ChessAPI instance
 */
const ChessBoard = ({ api }) => {
  const [fen, setFen] = useState('');
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);
  const [moveFrom, setMoveFrom] = useState('');
  const [gameStatus, setGameStatus] = useState('');
  const [currentTurn, setCurrentTurn] = useState('white');
  const [snackbar, setSnackbar] = useState({ message: '', type: 'error', visible: false });

  // Show a snackbar message
  const showSnackbar = useCallback((message, type = 'error') => {
    setSnackbar({ message, type, visible: true });
  }, []);
  
  // Clear the snackbar
  const clearSnackbar = useCallback(() => {
    setSnackbar({ message: '', type: 'error', visible: false });
  }, []);

  // Load the board state from API
  const fetchBoard = useCallback(async () => {
    try {
      setLoading(true);
      const data = await api.getBoard();
      console.log('Board data:', data);
      
      // Check if we have a valid response
      if (!data || data.status === 'error') {
        console.error('Invalid board data from API:', data);
        const errorMsg = data?.error?.message || 'Invalid board data from API';
        showSnackbar(errorMsg);
        return;
      }
      
      // Extract and validate FEN string
      // The API response structure is { status: 'success', data: { fen: '...' } }
      const fenString = data.data?.fen;
      console.log('Received initial FEN:', fenString);
      
      if (fenString && typeof fenString === 'string') {
        console.log('Setting initial FEN to:', fenString);
        setFen(fenString);
        
        // Determine current turn from FEN
        const fenParts = fenString.split(' ');
        if (fenParts.length >= 2) {
          setCurrentTurn(fenParts[1] === 'w' ? 'white' : 'black');
        }
      } else {
        console.error('Invalid FEN string received:', fenString);
        showSnackbar('Invalid FEN string received from API');
        return;
      }
      
      setError(null);
    } catch (err) {
      console.error('Error fetching board:', err);
      showSnackbar('Failed to load chess board: ' + err.message);
    } finally {
      setLoading(false);
    }
  }, [api, showSnackbar]);

  // Initial load
  useEffect(() => {
    fetchBoard();
  }, [fetchBoard]);

  // Reset the game
  const resetGame = async () => {
    try {
      setLoading(true);
      const data = await api.resetGame();
      console.log('Reset data:', data);
      
      // Check if we have a valid response
      if (!data || data.status === 'error') {
        console.error('Invalid reset data from API:', data);
        const errorMsg = data?.error?.message || 'Invalid reset data from API';
        showSnackbar(errorMsg);
        return;
      }
      
      // Extract and validate FEN string
      // The API response structure is { status: 'success', data: { fen: '...' } }
      const fenString = data.data?.fen;
      console.log('Received reset FEN:', fenString);
      
      if (fenString && typeof fenString === 'string') {
        console.log('Setting FEN after reset to:', fenString);
        setFen(fenString);
        
        // Determine current turn from FEN
        const fenParts = fenString.split(' ');
        if (fenParts.length >= 2) {
          setCurrentTurn(fenParts[1] === 'w' ? 'white' : 'black');
        }
        
        // After setting FEN, force a re-render by setting a dummy state
        setTimeout(() => {
          console.log('Forcing board update with reset FEN:', fenString);
          setLoading(false); // Toggle loading to force re-render
          setLoading(true);
          setLoading(false);
        }, 100);
      } else {
        console.error('Invalid FEN string received for reset:', fenString);
        showSnackbar('Invalid FEN string received from API for reset');
        return;
      }
      
      setGameStatus('');
      setMoveFrom('');
      setError(null);
      showSnackbar('Game reset successfully', 'success');
    } catch (err) {
      console.error('Error resetting game:', err);
      showSnackbar('Failed to reset game: ' + err.message);
    } finally {
      setLoading(false);
    }
  };

  // Make a move through the API
  const makeMove = async (from, to) => {
    try {
      setLoading(true);
      const moveString = `${from}${to}`;
      console.log('Sending move to API:', moveString);
      
      const data = await api.makeMove(moveString);
      console.log('Move response:', data);
      
      // Check if we have a valid response
      if (!data) {
        console.error('Invalid response from API:', data);
        showSnackbar('Invalid response from API');
        return false;
      }
      
      // Check for error response
      if (data.status === 'error') {
        console.error('Error from API:', data.error);
        showSnackbar(data.error?.message || 'Error making move');
        return false;
      }
      
      // Extract and validate FEN string
      // The API response structure is { status: 'success', data: { fen: '...' } }
      const fenString = data.data?.fen;
      console.log('Received FEN:', fenString);
      
      if (fenString && typeof fenString === 'string') {
        console.log('Setting FEN to:', fenString);
        setFen(fenString);
        
        // Determine current turn from FEN
        const fenParts = fenString.split(' ');
        if (fenParts.length >= 2) {
          setCurrentTurn(fenParts[1] === 'w' ? 'white' : 'black');
        }
        
        // After setting FEN, force a re-render by setting a dummy state
        setTimeout(() => {
          console.log('Forcing board update with FEN:', fenString);
          setLoading(false); // Toggle loading to force re-render
          setLoading(true);
          setLoading(false);
        }, 100);
      } else {
        console.error('Invalid FEN string received:', fenString);
        showSnackbar('Invalid FEN string received from API');
        return false;
      }
      
      // Reset game status since we don't have metadata anymore
      setGameStatus('');
      
      return true;
    } catch (err) {
      console.error('Error making move:', err);
      showSnackbar('Failed to make move: ' + err.message);
      return false;
    } finally {
      setLoading(false);
    }
  };

  // Check if a piece is a black piece
  const isBlackPiece = (piece) => {
    return piece && piece.toLowerCase() === piece;
  };

  // Handle square click
  const onSquareClick = (square) => {
    if (loading) return;
    
    // Only allow playing as white
    if (currentTurn !== 'white') {
      showSnackbar('It\'s not your turn');
      return;
    }

    // Get the piece at the clicked square
    const boardPosition = fenToObj(fen);
    const piece = boardPosition[square];

    // If no moveFrom is set and clicked on a black piece, do nothing
    if (!moveFrom && isBlackPiece(piece)) {
      showSnackbar('You can only move white pieces');
      return;
    }

    if (moveFrom) {
      // If same square clicked twice, deselect it
      if (moveFrom === square) {
        setMoveFrom('');
        return;
      }
      
      // Make the move
      makeMove(moveFrom, square);
      setMoveFrom('');
    } else {
      // Set the from square (only for white pieces)
      setMoveFrom(square);
    }
  };

  // Handle drag and drop
  const onPieceDrop = (from, to) => {
    if (loading) return false;
    
    // Only allow playing as white
    if (currentTurn !== 'white') {
      showSnackbar('It\'s not your turn');
      return false;
    }
    
    // Make the move
    return makeMove(from, to);
  };

  // Determine if a piece is draggable (only white pieces)
  const isDraggablePiece = ({ piece }) => {
    // White pieces are uppercase in FEN notation
    return piece && piece.toUpperCase() === piece;
  };

  if (loading && !fen) {
    return <div className="chess-loading">Loading...</div>;
  }

  return (
    <div className="chess-board-container">
      {snackbar.visible && (
        <Snackbar 
          message={snackbar.message} 
          type={snackbar.type} 
          onClose={clearSnackbar} 
        />
      )}
      
      <div className="chess-controls">
        <button className="chess-reset-button" onClick={resetGame}>Reset Game</button>
        {gameStatus && <div className="game-status">{gameStatus}</div>}
        <div className="turn-indicator">Turn: {currentTurn === 'white' ? 'White' : 'Black'}</div>
      </div>
      
      <div className="chess-board-wrapper">
        <Chessboard 
          position={fen}
          onSquareClick={onSquareClick}
          onPieceDrop={onPieceDrop}
          boardWidth={480}
          isDraggablePiece={isDraggablePiece}
          customSquareStyles={{
            ...(moveFrom && { [moveFrom]: { backgroundColor: 'rgba(255, 255, 0, 0.4)' } })
          }}
        />
      </div>
    </div>
  );
};

export default ChessBoard;
