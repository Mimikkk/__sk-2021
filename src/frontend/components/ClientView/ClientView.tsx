import { isUninitialized, useUserContext } from 'hooks';
import './ClientView.scss';
import { Button, TextField } from '@mui/material';
import { LoginScreen } from './components';
import { useState } from 'react';
import { useConnectionContext } from 'hooks/useConnection';

export const ClientView = () => {
  const { status, username, message, messages } = useUserContext();
  const { currentUser } = useConnectionContext();

  if (isUninitialized(status)) {
    return <LoginScreen />;
  }

  const [current, setCurrent] = useState('');

  console.log({ currentUser, current, messages, status, username });
  return (
    <div
      style={{
        display: 'flex',
        flexDirection: 'column',
        width: '100%',
        height: '100%',
        boxSizing: 'border-box',
        padding: '2rem',
        paddingLeft: '0',
        paddingBottom: '2.5rem',
      }}
    >
      <div
        style={{
          display: 'flex',
          flexDirection: 'column',
          maxWidth: '100%',
          height: '100%',
        }}
      >
        <div
          style={{
            display: 'flex',
            flexDirection: 'column',
            maxWidth: '100%',
            height: '600px',
            rowGap: '4px',
            overflowY: 'scroll',
          }}
        >
          {messages
            .filter(
              ({ messenger, recipient }) =>
                (recipient === currentUser && messenger === username) ||
                (recipient === username && messenger === currentUser),
            )
            .map(({ contents, messenger }) =>
              currentUser === messenger ? (
                <div className="receiver">{contents}</div>
              ) : (
                <div className="messenger">{contents}</div>
              ),
            )}
        </div>
      </div>
      <div
        style={{
          display: 'flex',
          columnGap: '8px',
          height: '6%',
          justifyItems: 'center',
          alignItems: 'center',
        }}
      >
        <TextField
          type="text"
          fullWidth
          value={current}
          onChange={({ target: { value } }) => setCurrent(value)}
          label={
            currentUser === username
              ? 'Cannot message self'
              : currentUser
              ? `Message from ${username} to ${currentUser}`
              : 'Select an user'
          }
          disabled={currentUser === username}
        />
        <Button
          variant="contained"
          onClick={() => {
            message(current, currentUser!);
          }}
          disabled={!current || !currentUser}
        >
          Send
        </Button>
      </div>
    </div>
  );
};
