import './App.scss';
import { useSocket } from './hooks';
import { isConnected } from './hooks/useSocket/socket-status';
import { Logger } from './components/devtools/Logger';

export const App = () => {
  const { socket, status } = useSocket('ws://localhost:9090');

  return (
    <>
      <div className="app">
        <header className="app-header">
          <p>React</p>
          <button
            type="button"
            onClick={() => {
              console.log(status);
              if (isConnected(status)) {
                socket.onopen = () => console.log('Connected');
                socket.onclose = () => console.log('Close');
              } else {
                console.log('Trying to close.');
                socket.close();
              }
            }}
          >
            Socket is {status}
          </button>
          <p>Socket on {socket.url}.</p>
        </header>
        <Logger />
      </div>
    </>
  );
};
