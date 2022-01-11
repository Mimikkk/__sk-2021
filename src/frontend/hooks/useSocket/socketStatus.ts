export enum SocketStatus {
  Connecting = 'Connecting',
  Connected = 'Open',
  Disconnecting = 'Disconnecting',
  Disconnected = 'Disconnected',
  Uninitialized = 'Uninitialized',
}

export const toStatus = (readyState: number) => {
  switch (readyState) {
    case WebSocket.CONNECTING:
      return SocketStatus.Connecting;
    case WebSocket.OPEN:
      return SocketStatus.Connected;
    case WebSocket.CLOSING:
      return SocketStatus.Disconnecting;
    case WebSocket.CLOSED:
      return SocketStatus.Disconnected;
    default:
      throw Error('Invalid readyState.');
  }
};

export const isConnecting = (status: SocketStatus) =>
  status === SocketStatus.Connecting;
export const isConnected = (status: SocketStatus) =>
  status === SocketStatus.Connected;
export const isDisconnecting = (status: SocketStatus) =>
  status === SocketStatus.Disconnecting;
export const isDisconnected = (status: SocketStatus) =>
  status === SocketStatus.Disconnected;
export const isUninitialized = (status: SocketStatus) =>
  status === SocketStatus.Uninitialized;
