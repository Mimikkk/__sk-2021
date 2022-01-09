import { useState } from 'react';
import { useSocket, isConnected, isDisconnected, useServerInfo } from 'hooks';
import faker from 'faker';
import { sample } from 'lodash';
import './ClientView.scss';

export const ClientView = () => {
  const { users } = useServerInfo();
  const [name, setName] = useState<string>('');
  const { status, connect, disconnect, send } = useSocket(name);

  return (
    <div className="client-view">
      <label>
        Name:
        <input
          onChange={({ target: { value } }) => setName(value)}
          value={name}
        />
      </label>

      <button
        onClick={() => {
          if (users.includes(name)) {
            console.log('User already logged in');
            return;
          }
          console.log('creating new connection with server');
          connect();
        }}
        disabled={isDisconnected(status) || !name}
      >
        Reconnect
      </button>
      <button
        onClick={() => {
          console.log('Closing connection with server');
          disconnect();
        }}
        disabled={!isConnected(status)}
      >
        Disconnect
      </button>
      <button
        type="button"
        onClick={() => {
          const user = sample(users);
          if (!user) return;

          send(faker.lorem.sentence(), user);
          console.log(`Sent random message to ${user}`);
        }}
        disabled={!isConnected(status)}
      >
        Send random sentence
      </button>
      <p>Connected users</p>
      <ul>
        {users.map((user) => (
          <li key={user}>{user}</li>
        ))}
      </ul>
    </div>
  );
};
