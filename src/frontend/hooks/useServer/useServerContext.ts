import { createContext, useContext } from 'react';

interface State {
  users: string[];
}
export const initial: State = {
  users: [],
};

export const ServerContext = createContext(initial);

export const useServerContext = () => useContext(ServerContext);
