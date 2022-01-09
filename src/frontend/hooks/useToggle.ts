import { useReducer } from 'react';

export const useToggle = () => useReducer((state) => !state, false);
