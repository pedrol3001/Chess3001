/**
 * Response from the Chess API for board state
 */
export interface BoardResponse {
  status: 'success' | 'error';
  data?: {
    fen: string;
    metadata?: {
      turn: 'white' | 'black';
      check: boolean;
      checkmate: boolean;
      [key: string]: any;
    };
  };
  error?: {
    code: string;
    message: string;
  };
}

/**
 * Response from the Chess API for move and reset operations
 */
export interface ApiResponse {
  status: 'success' | 'error';
  data?: {
    fen: string;
    [key: string]: any;
  };
  error?: {
    code: string;
    message: string;
  };
}

/**
 * Props for the ChessBoard component
 */
export interface ChessBoardProps {
  api: ChessAPIInterface;
}

/**
 * Interface for the ChessAPI class
 */
export interface ChessAPIInterface {
  baseUrl: string;
  getBoard(): Promise<BoardResponse>;
  makeMove(move: string): Promise<ApiResponse>;
  resetGame(): Promise<ApiResponse>;
}
