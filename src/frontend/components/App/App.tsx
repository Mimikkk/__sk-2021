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
        <ServerProvider>
          <ClientProvider>
            <Grid container spacing={2}>
              <Grid item xs={3} sx={{ borderRight: 1 }}>
                <UserList />
              </Grid>
              <Grid item xs={9}>
                <ClientView />
              </Grid>
            </Grid>
          </ClientProvider>
        </ServerProvider>
      </article>
    </div>
  );
};
