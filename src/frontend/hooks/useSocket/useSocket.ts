import { useCallback, useEffect, useRef } from 'react';

interface UseSocket {
  socket: WebSocket;
  reconnect: () => void;
}

export const useSocket = (url: string): UseSocket => {
  const socket = useRef(new WebSocket(url));

  const closeSocket = useCallback(socket.current.close, [socket]);

  const reconnect = useCallback(() => {
    socket.current = new WebSocket(url);

    const s = socket.current;
    s.addEventListener('message', (event) => {
      console.log({ event });
    });
  }, [url]);

  console.log('Fuck you?');
  useEffect(() => {
    reconnect();
    return closeSocket;
  }, [url]);

  return { socket: socket.current, reconnect } as const;
};
