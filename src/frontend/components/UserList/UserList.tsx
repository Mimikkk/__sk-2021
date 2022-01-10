import { useServerContext } from 'hooks';
import { Grid } from '@mui/material';
export const UserList = () => {
  const { users } = useServerContext();

  return (
    <Grid>
      <p>Currently Online</p>
      <ul>
        {users.map((user) => (
          <li key={user}>{user}</li>
        ))}
      </ul>
    </Grid>
  );
};
