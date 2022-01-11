import { Grid } from '@mui/material';
import React, { FC } from 'react';

export const Tile: FC = ({ children }) => (
  <Grid
    item
    style={{
      alignItems: 'center',
      alignContent: 'center',
      justifyContent: 'center',
      justifyItems: 'center',
      display: 'flex',
      width: '100%',
      height: '100%',
      paddingRight: '15%',
      paddingLeft: '15%',
      paddingTop: '5%',
      paddingBottom: '5%',
    }}
  >
    {children}
  </Grid>
);
