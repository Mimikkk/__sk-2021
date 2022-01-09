import { useCallback, useEffect, useRef, useState } from 'react';
import { isConnected, SocketStatus } from './socketStatus';
import { Nullable } from 'utils';
import { useToggle } from 'hooks';
import { commandService } from 'services';

const server_url = import.meta.env.VITE_SERVER_URL;

export interface UseSocketReturn {
  name: string;
  socket: Nullable<WebSocket>;
  status: SocketStatus;
  connect: () => void;
  disconnect: () => void;
  send: (message: string, recipient: string) => void;
}

export const useSocket = (newName: string): UseSocketReturn => {
  const [name, setName] = useState('');
  const socket = useRef<Nullable<WebSocket>>(null);

  const [status, setStatus] = useState(SocketStatus.Uninitialized);
  const [shouldConnect, connect] = useToggle();

  useEffect(() => {
    if (socket.current) disconnect();
    if (newName) {
      setName(newName);
      socket.current = new WebSocket(`ws://${server_url}/${newName}`);
      setStatus(SocketStatus.Connecting);

      socket.current.addEventListener('close', () =>
        setStatus(SocketStatus.Disconnected),
      );
      socket.current.addEventListener('open', () =>
        setStatus(SocketStatus.Connected),
      );
    }
  }, [shouldConnect]);

  const send = useCallback(
    (message, recipient) => {
      if (isConnected(status)) {
        commandService.send(socket.current!, message, recipient);
      }
    },
    [status],
  );

  const disconnect = useCallback(() => {
    if (isConnected(status)) {
      commandService.close(socket.current!);
      socket.current = null;
      setStatus(SocketStatus.Disconnecting);
      setName('');
    }
  }, [status]);

  return {
    socket: socket.current,
    name,
    status,
    connect,
    disconnect,
    send,
  } as const;
};
