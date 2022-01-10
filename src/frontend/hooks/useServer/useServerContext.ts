import { createContext, useContext } from 'react';

export interface State {
  users: string[];
}
export const initial: State = {
  users: [],
};

export const ServerContext = createContext(initial);

export const useServerContext = () => useContext(ServerContext);
