import { useServerContext } from 'hooks';
import {
  Collapse,
  Grid,
  List,
  ListItem,
  ListItemButton,
  ListItemIcon,
  ListItemText,
  Typography,
} from '@mui/material';
import { useState } from 'react';
import { Nullable } from 'utils';
import PersonIcon from '@mui/icons-material/Person';
import ExpandLess from '@mui/icons-material/ExpandLess';
import ExpandMore from '@mui/icons-material/ExpandMore';
import SendIcon from '@mui/icons-material/Send';
export const UserList = () => {
  const [current, setCurrent] = useState<Nullable<string>>(null);
  const { users } = useServerContext();

  return (
    <Grid spacing={2} flexDirection="column">
      <Grid item xs={12}>
        <Typography>Currently Online</Typography>
      </Grid>
      <Grid item xs={12}>
        <List>
          {users.map((user) => (
            <>
              <ListItemButton
                onClick={() => {
                  if (current !== user) {
                    setCurrent(user);
                  } else {
                    setCurrent(null);
                  }
                }}
              >
                <ListItemIcon>
                  <PersonIcon />
                </ListItemIcon>
                <ListItemText primary={user} />
                {current === user ? <ExpandLess /> : <ExpandMore />}
              </ListItemButton>
              <Collapse in={current === user} timeout="auto">
                <List component="div" disablePadding sx={{ pl: 4 }}>
                  <ListItemButton>
                    <ListItemIcon>
                      <SendIcon />
                    </ListItemIcon>
                    <ListItemText primary="message" />
                  </ListItemButton>
                </List>
              </Collapse>
            </>
          ))}
        </List>
      </Grid>
    </Grid>
  );
};
