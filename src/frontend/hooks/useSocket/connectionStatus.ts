export enum ConnectionStatus {
  Connecting = 'Connecting',
  Connected = 'Open',
  Disconnecting = 'Disconnecting',
  Disconnected = 'Disconnected',
  Uninitialized = 'Uninitialized',
}

export const isConnecting = (status: ConnectionStatus) =>
  status === ConnectionStatus.Connecting;
export const isConnected = (status: ConnectionStatus) =>
  status === ConnectionStatus.Connected;
export const isDisconnecting = (status: ConnectionStatus) =>
  status === ConnectionStatus.Disconnecting;
export const isDisconnected = (status: ConnectionStatus) =>
  status === ConnectionStatus.Disconnected;
export const isUninitialized = (status: ConnectionStatus) =>
  status === ConnectionStatus.Uninitialized;
