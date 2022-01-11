import { createContext, useContext } from 'react';
import { Message } from 'models';
import { noop } from 'lodash';
import { ConnectionStatus } from '../useSocket';

interface State {
  messages: Message.Model[];
  username: string;
  login: (name: string) => void;
  logoff: () => void;
  message: (message: string, recipient: string) => void;
  status: ConnectionStatus;
}

export const initial: State = {
  messages: [],
  username: '',
  login: noop,
  logoff: noop,
  message: noop,
  status: ConnectionStatus.Uninitialized,
};

export const UserContext = createContext(initial);

export const useUserContext = () => useContext(UserContext);
