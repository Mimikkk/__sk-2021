import { ServerCommand } from './types';
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
