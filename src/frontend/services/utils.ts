import { InfoEvent, MessageEvent, CloseEvent, ServerEvent } from './types';

const isServerEvent = (item: any): item is ServerEvent => 'type' in item;

export const isMessageEvent = (item: any): item is MessageEvent =>
  isServerEvent(item) && item.type === 'message';
export const isInfoEvent = (item: any): item is InfoEvent =>
  isServerEvent(item) && item.type === 'info';
export const isCloseEvent = (item: any): item is CloseEvent =>
  isServerEvent(item) && item.type === 'close';
