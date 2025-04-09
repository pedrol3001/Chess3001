import React, { useState, useEffect } from 'react';
import './App.css';
import { ChessBoard, ChessAPI } from '@chess3001/react-components';

const App: React.FC = () => {
  const [apiStatus, setApiStatus] = useState<string>('Connecting to Chess API...');
  const [apiConnected, setApiConnected] = useState<boolean>(false);
  
  // Create a single instance of the API with the correct URL
  // Using direct URL with port as per memory about CORS and networking issues
  const api = new ChessAPI('http://localhost:4567');

  useEffect(() => {
    // Check if the API is running by trying to get the board state
    const checkApiConnection = async () => {
      try {
        const response = await api.getBoard();
        if (response.status === 'success') {
          setApiStatus('Connected to Chess API');
          setApiConnected(true);
        } else {
          setApiStatus(`Error connecting to Chess API: ${response.error}`);
        }
      } catch (error) {
        setApiStatus(`Failed to connect to Chess API: ${error instanceof Error ? error.message : String(error)}`);
      }
    };

    checkApiConnection();
  }, [api]);

  return (
    <div className="App">
      <header className="App-header">
        <h1>Chess3001</h1>
        <p className={apiConnected ? 'api-status connected' : 'api-status error'}>
          {apiStatus}
        </p>
      </header>
      
      <main className="App-content">
        {apiConnected ? (
          <ChessBoard api={api} />
        ) : (
          <div className="api-error">
            <p>Please make sure the Chess API is running on http://localhost:4567</p>
            <p>The API should have the following endpoints available:</p>
            <ul>
              <li>GET /api/board - Get the current board state</li>
              <li>POST /api/move - Make a move</li>
              <li>POST /api/reset - Reset the game</li>
            </ul>
          </div>
        )}
      </main>
      
      <footer className="App-footer">
        <p>Powered by Chess3001 Engine</p>
      </footer>
    </div>
  );
};

export default App;
