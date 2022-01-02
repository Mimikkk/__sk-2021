import './App.scss';
import { useSocket } from './hooks';
import { Logger } from './components/devtools/Logger';

const server_url = import.meta.env.VITE_SERVER_URL;

export const ClientView = () => {
  const { socket } = useSocket(`ws://${server_url}`);

  return (
    <div>
      <button
        type="button"
        onClick={() => {
          console.log('I love you..');
          socket.send(
            'I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..I love you..',
          );
        }}
      >
        Send lots of love
      </button>
      <button
        type="button"
        onClick={() => {
          console.log('Cheers love!');
          socket.send('Cheers love!');
        }}
      >
        Send some love
      </button>
      <p>
        Socket on {socket.url} and is {status}
      </p>
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
