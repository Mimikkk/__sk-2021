export enum ServerCommand {
  Info = '<>server_info<>',
  Message = '<>message<>',
  Close = '<>close_connection<>',
}

export enum ServerEventType {
  Info = 'info',
  Message = 'message',
  Close = 'close',
}

export interface ServerEvent {
  type: ServerEventType;
}

export interface ServerInfo extends ServerEvent {
  names: string[];
}

export interface ServerMessage extends ServerEvent {
  messenger: string;
  message: string;
}

interface Service {
  requestServerInfo: (socket: WebSocket) => void;
  send: (socket: WebSocket, message: string, recipient: string) => void;
  close: (socket: WebSocket) => void;
}

export const commandService: Service = {
  requestServerInfo(socket) {
    socket.send(ServerCommand.Info);
  },
  send(socket, message, recipient) {
    socket.send(`${ServerCommand.Message}<${recipient}|${message}>`);
  },
  close(socket) {
    socket.send(ServerCommand.Close);
    socket.close();
  },
};
