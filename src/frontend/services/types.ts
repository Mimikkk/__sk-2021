export enum ServerCommand {
  Info = '<>server_info<>',
  Message = '<>message<>',
  Close = '<>close_connection<>',
}

export interface InfoEvent {
  names: string[];
  type: 'info';
}

export interface MessageEvent {
  messenger: string;
  message: string;
  type: 'message';
}

export interface CloseEvent {
  type: 'close';
}

export type ServerEvent = InfoEvent | MessageEvent | CloseEvent;
