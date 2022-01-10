import { Button, Grid, TextField, Typography } from '@mui/material';
import { useState } from 'react';
import { useUserContext } from 'hooks';

export const LoginScreen = () => {
  const [name, setName] = useState<string>('');
  const { login } = useUserContext();

  return (
    <Grid
      container
      flexDirection="column"
      spacing={2}
      justifyContent="center"
      alignItems="center"
    >
      <Grid item>
        <Typography variant="h3">Welcome!</Typography>
      </Grid>
      <Grid item>
        <TextField
          type="text"
          value={name}
          onChange={({ target: { value } }) => setName(value)}
          label="Username"
        />
      </Grid>
      <Grid item>
        <Button variant="outlined" onClick={() => login(name)} disabled={!name}>
          Try to connect
        </Button>
      </Grid>
    </Grid>
  );
};
