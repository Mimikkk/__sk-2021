import { Button, Grid, TextField } from '@mui/material';
import { useState } from 'react';
import { useUserContext } from 'hooks';

export const LoginScreen = () => {
  const [name, setName] = useState<string>('');
  const { login } = useUserContext();

  return (
    <Grid
      container
      flexDirection="column"
      justifyContent="center"
      alignItems="center"
      spacing={2}
    >
      <Grid item>Welcome!</Grid>
      <Grid item>
        <TextField
          type="text"
          value={name}
          onChange={({ target: { value } }) => setName(value)}
          label="Username"
        />
      </Grid>
      <Grid item>
        <Button
          variant="contained"
          onClick={() => login(name)}
          disabled={!name}
          style={{
            backgroundColor: '',
            color: 'white',
          }}
        >
          Log in
        </Button>
      </Grid>
    </Grid>
  );
};
