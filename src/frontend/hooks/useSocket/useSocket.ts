import { useCallback, useEffect, useRef, useState } from 'react';
import { isConnected, SocketStatus } from './socketStatus';
import { Nullable } from 'utils';
import { useToggle } from 'hooks';
import { commandService } from 'services';

const server_url = import.meta.env.VITE_SERVER_URL;

export interface UseSocketReturn {
  socket: Nullable<WebSocket>;
  status: SocketStatus;
  connect: () => void;
  disconnect: () => void;
  send: (message: string, recipient: string) => void;
}

export const useSocket = (name: string): UseSocketReturn => {
  const socketRef = useRef<Nullable<WebSocket>>(null);
  const socket = socketRef.current;

  const [status, setStatus] = useState(SocketStatus.Uninitialized);
  const [shouldConnect, connect] = useToggle();

  useEffect(() => {
    if (socketRef.current) disconnect();
    if (name) {
      socketRef.current = new WebSocket(`ws://${server_url}/${name}`);
      setStatus(SocketStatus.Connecting);

      socketRef.current.addEventListener('close', () =>
        setStatus(SocketStatus.Disconnected),
      );
      socketRef.current.addEventListener('open', () =>
        setStatus(SocketStatus.Connected),
      );

      const ws = socketRef.current;
      return () => ws?.close();
    }
  }, [shouldConnect]);

  const send = useCallback(
    (message, recipient) => {
      if (isConnected(status)) {
        commandService.send(socket!, message, recipient);
      }
    },
    [status],
  );

  const disconnect = useCallback(() => {
    if (isConnected(status)) {
      commandService.close(socket!);
      socketRef.current = null;
      setStatus(SocketStatus.Disconnecting);
    }
  }, [status]);

  return { socket, status, connect, disconnect, send } as const;
};
