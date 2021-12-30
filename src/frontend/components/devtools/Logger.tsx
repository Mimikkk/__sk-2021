import { useEffect, useRef, useState } from 'react';
import { cx } from '../../utils';
import './Logger.scss';

export const Logger = () => {
  const [old, setOld] = useState<(...data: any[]) => void>(() => {});
  const messages = useRef<string[]>([]);
  useEffect(() => {
    const logger = document.getElementById('log')!;
    setOld(console.log);

    console.log = (message) => {
      messages.current.push(message);
      if (messages.current.length > 8) messages.current.shift();
      logger.innerHTML = messages.current
        .reduce<string[]>((acc, message) => {
          acc.push(
            typeof message == 'object' ? JSON.stringify(message) : message,
          );
          return acc;
        }, [])
        .join('<br>');
    };
    return () => {
      console.log = old;
    };
  }, []);

  return <div id="log" className={cx('logger')} />;
};
