import { ClientView, Tile, UserList } from 'components';
import { useServer, useUser } from 'hooks';
import './App.scss';
import { Card, Grid } from '@mui/material';
import { useConnection } from '../../hooks/useConnection';

export const App = () => {
  const [ServerProvider] = useServer();
  const [ClientProvider] = useUser();
  const [ConnectionProvider] = useConnection();

  return (
    <div className="app">
      <article>
        <Tile>
          <Card
            sx={{
              width: '100%',
              height: '100%',
              elevation: 4,
              borderRadius: '20px',
            }}
          >
            <Grid container style={{ height: '100%', width: '100%' }}>
              <ConnectionProvider>
                <Grid container item xs={3}>
                  <Grid item xs={11}>
                    <ServerProvider>
                      <UserList />
                    </ServerProvider>
                  </Grid>
                  <Grid
                    item
                    xs={1}
                    sx={{
                      borderLeft: 1,
                      height: '92%',
                      alignSelf: 'center',
                      borderLeftColor: 'rgb(157, 161, 168)',
                    }}
                  />
                </Grid>
                <Grid container item xs={9}>
                  <ClientProvider>
                    <ClientView />
                  </ClientProvider>
                </Grid>
              </ConnectionProvider>
            </Grid>
          </Card>
        </Tile>
      </article>
    </div>
  );
};
