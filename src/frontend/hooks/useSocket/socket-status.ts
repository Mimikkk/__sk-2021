export enum SocketStatus {
  Connecting = 'Connecting',
  Connected = 'Open',
  Disconnecting = 'Disconnecting',
  Disconnected = 'Disconnected',
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

export const isConnecting = (state: SocketStatus) =>
  state === SocketStatus.Connecting;
export const isConnected = (state: SocketStatus) =>
  state === SocketStatus.Connected;
export const isDisconnecting = (state: SocketStatus) =>
  state === SocketStatus.Disconnecting;
export const isDisconnected = (state: SocketStatus) =>
  state === SocketStatus.Disconnected;
