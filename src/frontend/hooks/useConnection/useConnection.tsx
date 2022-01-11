import { useCallback, useState } from 'react';
import { Nullable } from '../../utils';
import { ConnectionContext } from './useConnectionContext';

export const useConnection = () => {
  const [currentUser, setCurrentUser] = useState<Nullable<string>>(null);
  const [users, setUsers] = useState<string[]>([]);

  const Provider = useCallback(
    ({ children }) => (
      <ConnectionContext.Provider
        value={{ currentUser, setCurrentUser, users, setUsers }}
      >
        {children}
      </ConnectionContext.Provider>
    ),
    [currentUser, users],
  );
  return [Provider] as const;
};
