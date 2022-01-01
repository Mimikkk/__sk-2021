import './App.scss';
import { useSocket } from './hooks';
import { isConnected, isDisconnected } from './hooks/useSocket/socket-status';
import { Logger } from './components/devtools/Logger';

const server_url = import.meta.env.VITE_SERVER_URL;

export const ClientView = () => {
  const { socket, status } = useSocket(`ws://${server_url}`);

  return (
    <div>
      <button
        type="button"
        onClick={() => {
          console.log(status);
          if (isDisconnected(status)) {
            socket.onopen = () => console.log('Connected');
            socket.onclose = () => console.log('Close');
            socket.onmessage = console.log;
            socket.onerror = console.log;
          } else if (isConnected(status)) {
            socket.close();
          }
        }}
      >
        Socket is {status}
      </button>
      <p>Socket on {socket.url}.</p>
    </div>
  );
};

export const App = () => {
  return (
    <div className="app">
      <header>Title</header>
      <article>
        <ClientView />
      </article>
      <Logger />
    </div>
  );
};
