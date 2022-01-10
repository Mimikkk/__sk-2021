import './App.scss';
import { ClientView } from 'components/ClientView';
import { useServer } from '../../hooks/useServerContext';

export const App = () => {
  const [ServerProvider] = useServer();

  return (
    <div className="app">
      <header>sk-chat</header>
      <article>
        <ServerProvider>
          <ClientView />
        </ServerProvider>
      </article>
    </div>
  );
};
