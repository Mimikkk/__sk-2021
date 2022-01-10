import { isUninitialized, useUserContext } from 'hooks';
import './ClientView.scss';
import { Grid, Typography } from '@mui/material';
import { LoginScreen } from './components';

export const ClientView = () => {
  const { status, username } = useUserContext();

  if (isUninitialized(status)) {
    return <LoginScreen />;
  }

  return (
    <Grid container flexDirection="column" spacing={2}>
      <Grid item>
        <Typography>logged as {username}</Typography>
      </Grid>
    </Grid>
  );
};
