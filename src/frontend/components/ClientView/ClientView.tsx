import React, { ChangeEvent, useEffect, useRef } from 'react';
import { commandService } from 'services';
import faker from 'faker';
import { isDisconnected, toStatus } from 'hooks/useSocket/socket-status';
import { Nullable } from 'utils';
import './ClientView.scss';

const server_url = import.meta.env.VITE_SERVER_URL;

export const ClientView = () => {
  const socket = useRef<Nullable<WebSocket>>(null);

  useEffect(() => {
    if (socket.current) {
      socket.current.onmessage = (event) => {
        console.log({ event });
      };
      socket.current.onerror = (event) => {
        console.log({ event });
      };
      socket.current.onclose = (event) => {
        console.log({ event });
      };
      socket.current.onopen = (event) => {
        console.log({ event });
      };
    }
  }, [socket.current]);

  const [name, setName] = React.useState<string>('');

  const handleChange = ({
    target: { value },
  }: ChangeEvent<HTMLInputElement>) => {
    console.log(`Input change ${value}`);
    setName(value);
  };

  return (
    <div className="client-view">
      <label>
        Name:
        <input onChange={handleChange} value={name} />
      </label>

      <button
        onClick={() => {
          socket.current = new WebSocket(`ws://${server_url}/${name}`);
        }}
        disabled={
          !isDisconnected(toStatus(socket.current?.readyState || 3)) || !name
        }
      >
        Reconnect
      </button>
      <button
        type="button"
        onClick={() => {
          commandService.sendMessage(
            socket.current!,
            faker.lorem.sentence(),
            'monika',
          );
          console.log('Send random message');
        }}
        disabled={!socket.current}
      >
        Send random sentence
      </button>
      <button
        type="button"
        onClick={() => {
          commandService.readServerInfo(socket.current!);
          console.log('Requested server info');
        }}
        disabled={!socket.current}
      >
        Request server info
      </button>
      <p>lol</p>
    </div>
  );
};
