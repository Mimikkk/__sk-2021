import { createContext, useContext } from 'react';
import { Nullable } from '../../utils';

interface State {
  users: string[];
  currentUser: Nullable<string>;
  setCurrentUser(user: Nullable<string>): void;
  setUsers(users: string[]): void;
}
export const initial: State = {
  users: [],
  currentUser: null,
  setCurrentUser: () => {},
  setUsers: () => {},
};

export const ConnectionContext = createContext(initial);

export const useConnectionContext = () => useContext(ConnectionContext);
