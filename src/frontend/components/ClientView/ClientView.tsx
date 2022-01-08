import React, { ChangeEvent, useEffect } from 'react';
import { commandService } from 'services';
import faker from 'faker';

const server_url = import.meta.env.VITE_SERVER_URL;
const socket = new WebSocket(`ws://${server_url}/name`);

export const ClientView = () => {
  useEffect(() => {
    socket.onmessage = (event) => {
      console.log({ event });
    };
    socket.onerror = (event) => {
      console.log({ event });
    };
    socket.onclose = (event) => {
      console.log({ event });
    };
    socket.onopen = (event) => {
      console.log({ event });
    };
  });

  const [name, setName] = React.useState<string>('');

  const handleChange = ({
    target: { value },
  }: ChangeEvent<HTMLInputElement>) => {
    console.log(`Input change ${value}`);
    setName(value);
  };

  return (
    <div
      style={{
        display: 'flex',
        justifyContent: 'center',
        alignContent: 'center',
        flexDirection: 'column',
      }}
    >
      <input onChange={handleChange} />
      <div>Name: {name}</div>

      <button
        type="button"
        onClick={() => {
          commandService.sendMessage(socket, faker.lorem.sentence(), 'monika');
          console.log('Send random message');
        }}
      >
        Send random sentence
      </button>
      <button
        type="button"
        onClick={() => {
          commandService.readServerInfo(socket);
          console.log('Requested server info');
        }}
      >
        Request server info
      </button>
      <p>lol</p>
    </div>
  );
};
