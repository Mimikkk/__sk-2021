import { useEffect, useState } from 'react';
import { useSocket, isConnected, isDisconnected, useServerInfo } from 'hooks';
import faker from 'faker';
import { sample, without } from 'lodash';
import './ClientView.scss';
import { isMessageEvent } from '../../services';

export module Message {
  export interface Model {
    messenger: string;
    contents: string;
  }
}

export const ClientView = () => {
  const { users } = useServerInfo();
  const [newName, setNewName] = useState<string>('');
  const [messages, setMessages] = useState<Message.Model[]>([]);
  const { socket, status, name, connect, disconnect, send } =
    useSocket(newName);

  useEffect(() => {
    if (isConnected(status)) {
      socket!.addEventListener('message', ({ data }) => {
        data = JSON.parse(data);

        if (isMessageEvent(data)) {
          setMessages((messages) => [
            ...messages,
            { contents: data.message, messenger: data.messenger },
          ]);
        }
      });
    }
  }, [status]);

  return (
    <div className="client-view">
      {name ? <header>Logged in as {name}</header> : 'Not logged in'}

      <label>
        Name:
        <input
          onChange={({ target: { value } }) => setNewName(value)}
          value={newName}
        />
      </label>

      <button
        onClick={() => {
          if (users.includes(newName)) {
            console.log('User already logged in');
            return;
          }
          console.log('creating new connection with server');
          connect();
        }}
        disabled={isDisconnected(status) || !newName}
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
          const user = sample(without(users, newName));
          if (!user) return;

          send(faker.lorem.sentence(), user);
          console.log(`Sent random message to ${user}`);
        }}
        disabled={!isConnected(status)}
      >
        Send random sentence
      </button>
      <div style={{ display: 'flex', flexDirection: 'row', columnGap: '1em' }}>
        <div style={{ display: 'flex', flexDirection: 'column' }}>
          <p>Connected users</p>
          <ul>
            {users.map((user) => (
              <li key={user}>{user}</li>
            ))}
          </ul>
        </div>
        <div style={{ display: 'flex', flexDirection: 'column' }}>
          <p>Messages</p>
          <ul>
            {messages.map((message) => (
              <li key={message.contents}>
                <span>{message.messenger}</span>
                <span> sent </span>
                <span>{message.contents}</span>
              </li>
            ))}
          </ul>
        </div>
      </div>
    </div>
  );
};
