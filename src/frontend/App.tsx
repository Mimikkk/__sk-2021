import {useRef, useState} from 'react'
import logo from './logo.svg'
import './App.css'

const App = () => {
  const socket = useRef(new WebSocket('ws://localhost:9090'));

  return (
    <div className="App">
      <header className="App-header">
        <p>React</p>
        <p>
          <button type="button" onClick={() => console.log(socket)}>
              Socket
          </button>
        </p>
          <p>Socket connected to {socket.current.url}.</p>
      </header>
    </div>
  )
};

export default App
