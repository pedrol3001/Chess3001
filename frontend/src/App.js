import React from 'react';
import './App.css';
import { ChessBoard, ChessAPI } from '@pedrol3001/chess3001-react-components';

function App() {
  // Create a single instance of the API
  const api = new ChessAPI();

  return (
    <div className="App">
      <header className="App-header">
        <h1>Chess3001</h1>
      </header>
      
      <main className="App-content">
        <ChessBoard api={api} />
      </main>
      
      <footer className="App-footer">
        <p>Powered by Chess3001 Engine</p>
      </footer>
    </div>
  );
}

export default App;
