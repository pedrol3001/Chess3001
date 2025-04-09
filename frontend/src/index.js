import React from 'react';
import ReactDOM from 'react-dom/client';
import './index.css';
import App from './App';

// Make sure the Chess API is running on http://localhost:4567
// before starting the React app

const root = ReactDOM.createRoot(document.getElementById('root'));
root.render(
  <React.StrictMode>
    <App />
  </React.StrictMode>
);
