import { createContext, useContext } from 'react';
import { Message } from 'models';

export interface State {
  messages: Message.Model[];
}
export const initial: State = {
  messages: [],
};

export const UserContext = createContext(initial);

export const useUserContext = () => useContext(UserContext);
