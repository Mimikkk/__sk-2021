import { isConnected, useSocket } from '../useSocket';
import { UserContext } from './useUserContext';
import { useCallback, useEffect, useState } from 'react';
import { Message } from 'models';
import { isMessageEvent } from 'services';

export const useUser = () => {
  const [username, setUsername] = useState('');
  const [messages, setMessages] = useState<Message.Model[]>([]);
  const { socket, status, name, connect, send, disconnect } =
    useSocket(username);

  const login = (name: string) => {
    setUsername(name);
    connect();
  };

  const logoff = () => {
    setUsername('');
    setMessages([]);
    disconnect();
  };

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

  const Provider = useCallback(
    ({ children }) => (
      <UserContext.Provider
        value={{ messages, username, login, logoff, message: send, status }}
      >
        {children}
      </UserContext.Provider>
    ),
    [name],
  );

  return [Provider] as const;
};
