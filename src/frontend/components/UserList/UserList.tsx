import { useServerContext } from 'hooks';
import {
  createTheme,
  Grid,
  List,
  ListItemButton,
  ListItemIcon,
  ListItemText,
  ThemeProvider,
  Typography,
} from '@mui/material';
import PersonIcon from '@mui/icons-material/Person';
import { useConnectionContext } from 'hooks/useConnection';

const theme = createTheme({
  typography: {
    fontFamily: ['Quicksand', 'sans-serif'].join(','),
  },
});
export const UserList = () => {
  const { currentUser, setCurrentUser } = useConnectionContext();
  const { users } = useServerContext();

  if (users.length === 0) {
    return null;
  }

  return (
    <Grid flexDirection="column" padding="5%" paddingTop="8%">
      <Grid item xs={12}>
        <ThemeProvider theme={theme}>
          <Typography style={{ fontSize: '1em' }}>
            Users online {users.length}
          </Typography>
        </ThemeProvider>
      </Grid>
      <Grid item xs={12}>
        <List>
          {users.map((user) => (
            <>
              <ListItemButton
                onClick={() => {
                  if (currentUser !== user) {
                    setCurrentUser(user);
                  } else {
                    setCurrentUser(null);
                  }
                }}
              >
                <ListItemIcon>
                  <PersonIcon
                    color={currentUser === user ? 'primary' : 'disabled'}
                  />
                </ListItemIcon>
                <ListItemText primary={user} />
              </ListItemButton>
            </>
          ))}
        </List>
      </Grid>
    </Grid>
  );
};
