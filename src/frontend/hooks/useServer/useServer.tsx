import { useServerInfo } from './useServerInfo';
import { useCallback } from 'react';
import { initial, ServerContext } from './useServerContext';

export const useServer = () => {
  const { users } = useServerInfo();

  const Provider = useCallback(
    ({ children }) => (
      <ServerContext.Provider value={initial}>
        {children}
      </ServerContext.Provider>
    ),
    [users],
  );

  return [Provider] as const;
};
