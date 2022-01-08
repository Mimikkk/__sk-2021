import './App.scss';
import { ClientView } from 'components/ClientView';

export const App = () => (
  <div className="app">
    <header>sk-chat</header>
    <article>
      <ClientView />
    </article>
  </div>
);
