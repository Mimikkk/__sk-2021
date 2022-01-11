import { ClientView, UserList } from 'components';
import { useServer, useUser } from 'hooks';
import './App.scss';
import { Grid } from '@mui/material';

export const App = () => {
  const [ServerProvider] = useServer();
  const [ClientProvider] = useUser();

  return (
    <div className="app">
      <article>
        <Grid container>
          <Grid item xs={3} sx={{ borderRight: 1 }}>
            <ServerProvider>
              <UserList />
            </ServerProvider>
          </Grid>
          <Grid container item xs={9} alignItems="center">
            <ClientProvider>
              <ClientView />
            </ClientProvider>
          </Grid>
        </Grid>
      </article>
    </div>
  );
};
