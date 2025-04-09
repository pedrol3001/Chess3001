/**
 * Chess API service for communicating with the Chess3001 API
 */
class ChessAPI {
  /**
   * Creates a new ChessAPI instance
   * @param {string} baseUrl - The base URL of the Chess API
   */
  constructor(baseUrl = 'http://localhost:4567') {
    this.baseUrl = baseUrl;
  }

  /**
   * Get the current board state
   * @returns {Promise<Object>} The current board state with status and data/error fields
   */
  async getBoard() {
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
      return data;
    } catch (error) {
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
   * @param {string} move - The move in algebraic notation (e.g., 'e2e4')
   * @returns {Promise<Object>} The move response with status and data/error fields
   */
  async makeMove(move) {
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
      return data;
    } catch (error) {
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
   * @returns {Promise<Object>} The reset response with status and data/error fields
   */
  async resetGame() {
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
      return data;
    } catch (error) {
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
