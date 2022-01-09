import { useEffect, useState } from 'react';
import { isConnected, isDisconnected, useSocket } from './useSocket';
import { commandService, isInfoEvent } from '../services';

export const useServerInfo = () => {
  const [users, setUsers] = useState<string[]>([]);
  const { socket, status, connect, disconnect } = useSocket('server');

  useEffect(() => {
    console.log('useServerInfo', status);

    if (isConnected(status)) {
      socket!.onmessage = ({ data }) => {
        data = JSON.parse(data);
        isInfoEvent(data) && setUsers(data.names);
      };

      const handle = setInterval(() => {
        if (isConnected(status)) {
          commandService.requestServerInfo(socket!);
        } else if (isDisconnected(status)) {
          connect();
        }
      }, 4000);

      return () => {
        clearInterval(handle);
        disconnect();
      };
    }

    // const handle = setInterval(() => {
    //   console.log('status', status);
    //   if (!isConnected(status)) {
    //     connect();
    //   }
    // }, 5000);
    // return () => {
    //   clearInterval(handle);
    // };
  }, [status]);

  return { users } as const;
};
