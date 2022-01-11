import { isUninitialized, useUserContext } from 'hooks';
import './ClientView.scss';
import { Button, Grid, TextField, Typography } from '@mui/material';
import { LoginScreen } from './components';
import { useState } from 'react';

export const ClientView = () => {
  const { status, username, message } = useUserContext();

  if (isUninitialized(status)) {
    return <LoginScreen />;
  }

  const [current, setCurrent] = useState('');
  return (
    <Grid container flexDirection="column" spacing={2}>
      <Grid item>
        <TextField
          type="text"
          value={current}
          onChange={({ target: { value } }) => setCurrent(value)}
          label="Message"
        />
        <Button onClick={() => message(current, '256')} disabled={!current}>
          Send
        </Button>
        <Typography>logged as {username}</Typography>
      </Grid>
    </Grid>
  );
};
