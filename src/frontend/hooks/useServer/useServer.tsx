import { useServerInfo } from './useServerInfo';
import { ServerContext } from './useServerContext';
import { FC } from 'react';

export const useServer = () => {
  const { users } = useServerInfo();

  const Provider: FC = ({ children }) => (
    <ServerContext.Provider value={{ users }}>
      {children}
    </ServerContext.Provider>
  );

  return [Provider] as const;
};
