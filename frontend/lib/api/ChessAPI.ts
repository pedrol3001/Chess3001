import { BoardResponse, ApiResponse, ChessAPIInterface } from '../types';

/**
 * Chess API service for communicating with the Chess3001 API
 */
class ChessAPI implements ChessAPIInterface {
  baseUrl: string;

  /**
   * Creates a new ChessAPI instance
   * @param baseUrl - The base URL of the Chess API
   */
  constructor(baseUrl = 'http://localhost:4567') {
    this.baseUrl = baseUrl;
  }

  /**
   * Get the current board state
   * @returns The current board state with status and data/error fields
   */
  async getBoard(): Promise<BoardResponse> {
    try {
      const response = await fetch(`${this.baseUrl}/api/board`, {
        method: 'GET',
        mode: 'cors',
        headers: {
          'Accept': 'application/json',
          'Content-Type': 'application/json'
        }
      });

      if (!response.ok) {
        throw new Error(`HTTP error! Status: ${response.status}`);
      }

      const data = await response.json();
      console.log('API response (getBoard):', data);
      
      // Return the full response which includes status and data/error fields
      return data as BoardResponse;
    } catch (error: any) {
      console.error('Error fetching board state:', error);
      // Return a standardized error response
      return {
        status: 'error',
        error: {
          code: 'network_error',
          message: `Failed to connect to the chess API: ${error.message}`
        }
      };
    }
  }

  /**
   * Make a move
   * @param move - The move in algebraic notation (e.g., 'e2e4')
   * @returns The move response with status and data/error fields
   */
  async makeMove(move: string): Promise<ApiResponse> {
    try {
      const response = await fetch(`${this.baseUrl}/api/move`, {
        method: 'POST',
        mode: 'cors',
        headers: {
          'Accept': 'application/json',
          'Content-Type': 'application/json'
        },
        body: JSON.stringify({ move })
      });

      if (!response.ok) {
        throw new Error(`HTTP error! Status: ${response.status}`);
      }

      const data = await response.json();
      console.log('API response (makeMove):', data);
      
      // Return the full response which includes status and data/error fields
      return data as ApiResponse;
    } catch (error: any) {
      console.error('Error making move:', error);
      // Return a standardized error response
      return {
        status: 'error',
        error: {
          code: 'network_error',
          message: `Failed to send move to the chess API: ${error.message}`
        }
      };
    }
  }

  /**
   * Reset the game
   * @returns The reset response with status and data/error fields
   */
  async resetGame(): Promise<ApiResponse> {
    try {
      const response = await fetch(`${this.baseUrl}/api/reset`, {
        method: 'POST',
        mode: 'cors',
        headers: {
          'Accept': 'application/json',
          'Content-Type': 'application/json'
        }
      });

      if (!response.ok) {
        throw new Error(`HTTP error! Status: ${response.status}`);
      }

      const data = await response.json();
      console.log('API response (resetGame):', data);
      
      // Return the full response which includes status and data/error fields
      return data as ApiResponse;
    } catch (error: any) {
      console.error('Error resetting game:', error);
      // Return a standardized error response
      return {
        status: 'error',
        error: {
          code: 'network_error',
          message: `Failed to reset the game: ${error.message}`
        }
      };
    }
  }
}

export default ChessAPI;
