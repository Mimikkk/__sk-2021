import { useEffect, useState } from 'react';
import { isConnected, isDisconnected, useSocket } from 'hooks';
import { commandService, isInfoEvent } from 'services';
import { isEqual } from 'lodash';

export const useServerInfo = () => {
  const [users, setUsers] = useState<string[]>([]);
  const { socket, status, connect } = useSocket('server');

  useEffect(() => {
    if (isConnected(status)) {
      socket!.onmessage = ({ data }) => {
        data = JSON.parse(data);
        if (isInfoEvent(data) && !isEqual(users, data.names))
          setUsers(data.names);
      };

      setInterval(() => {
        if (isConnected(status)) {
          commandService.requestServerInfo(socket!);
        } else if (isDisconnected(status)) {
          connect();
        }
      }, 4000);
    }
  }, [users, status]);

  return { users } as const;
};
