import React, { ChangeEvent, useEffect, useRef, useState } from 'react';
import {
  commandService,
  ServerEvent,
  ServerEventType,
  ServerInfo,
  ServerMessage,
} from 'services';
import faker from 'faker';
import { Nullable } from 'utils';
import './ClientView.scss';

const server_url = import.meta.env.VITE_SERVER_URL;

export const ClientView = () => {
  const socket = useRef<Nullable<WebSocket>>(null);
  const [users, setUsers] = useState<string[]>([]);

  useEffect(() => {
    if (socket.current) {
      socket.current.onmessage = (event) => {
        console.log({ event });

        let data = JSON.parse(event.data) as ServerEvent;
        if (data.type === ServerEventType.Info) {
          setUsers((data as ServerInfo).names);
        } else if (data.type === ServerEventType.Message) {
          let { messenger, message } = data as ServerMessage;
          console.log(`Message from ${messenger} with ${message}`);
        }
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

  const handleChange = ({ target: { value } }: ChangeEvent<HTMLInputElement>) =>
    setName(value);

  return (
    <div className="client-view">
      <label>
        Name:
        <input onChange={handleChange} value={name} />
      </label>

      <button
        onClick={() => {
          console.log('creating new connection with server');
          socket.current = new WebSocket(`ws://${server_url}/${name}`);
        }}
        // disabled={
        //   !isDisconnected(toStatus(socket.current?.readyState || 3)) || !name
        // }
      >
        Reconnect
      </button>
      <button
        onClick={() => {
          console.log('Closing connection with server');
          commandService.close(socket.current!);
          socket.current?.close();
        }}
        // disabled={!isConnected(toStatus(socket.current?.readyState || 3))}
      >
        Disconnect
      </button>
      <button
        type="button"
        onClick={() => {
          commandService.send(
            socket.current!,
            faker.lorem.sentence(),
            'monika',
          );
          console.log('Send random message');
        }}
        // disabled={!socket.current}
      >
        Send random sentence
      </button>
      <button
        type="button"
        onClick={() => {
          commandService.requestServerInfo(socket.current!);
          console.log('Requested server info');
        }}
        // disabled={!socket.current}
      >
        Request server info
      </button>
      <p>lol</p>
    </div>
  );
};
