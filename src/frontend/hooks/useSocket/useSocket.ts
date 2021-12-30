import { useEffect, useRef } from 'react';
import { SocketStatus, toStatus } from './socket-status';

interface UseSocket {
  socket: WebSocket;
  status: SocketStatus;
}
export const useSocket = (url: string): UseSocket => {
  const socket = useRef(new WebSocket(url));
  useEffect(() => {
    socket.current = new WebSocket(url);
  }, [url]);

  return {
    socket: socket.current,
    status: toStatus(socket.current.readyState),
  } as const;
};
