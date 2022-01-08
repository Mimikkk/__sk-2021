export enum ServerCommand {
  Info = '<>server_info<>',
  Message = '<>message<>',
}

interface Service {
  readServerInfo: (socket: WebSocket) => void;
  sendMessage: (socket: WebSocket, message: string, recipient: string) => void;
}

export const commandService: Service = {
  readServerInfo: (socket) => socket.send(ServerCommand.Info),
  sendMessage: (socket, message, recipient) =>
    socket.send(`${ServerCommand.Message}<${recipient}|${message}>`),
};
