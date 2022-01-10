import { useEffect, useState } from 'react';
import { useSocket, isConnected, isUninitialized } from 'hooks';
import './ClientView.scss';
import { isMessageEvent } from 'services';
import { Button, Grid, TextField, Typography } from '@mui/material';

export const ClientView = () => {
  const [newName, setNewName] = useState<string>('');
  const [messages, setMessages] = useState<Message.Model[]>([]);
  const { socket, status, name, connect, disconnect, send } =
    useSocket(newName);

  useEffect(() => {
    if (isConnected(status)) {
      socket!.addEventListener('message', ({ data }) => {
        data = JSON.parse(data);

        if (isMessageEvent(data)) {
          setMessages((messages) => [
            ...messages,
            { contents: data.message, messenger: data.messenger },
          ]);
        }
      });
    }
  }, [status]);

  if (isUninitialized(status)) {
    return (
      <Grid
        container
        flexDirection="column"
        spacing={2}
        justifyContent="center"
        alignItems="center"
      >
        <Grid item>
          <Typography variant="h3">Welcome to the chat!</Typography>
        </Grid>
        <Grid item>
          <TextField
            type="text"
            value={newName}
            onChange={({ target: { value } }) => setNewName(value)}
            label="Username"
          />
        </Grid>
        <Grid item>
          <Button
            variant="outlined"
            onClick={() => connect()}
            disabled={!newName}
          >
            Try to connect
          </Button>
        </Grid>
      </Grid>
    );
  }

  return (
    <Grid container flexDirection="column" spacing={2}>
      <Grid item>
        <Typography>Connected as {name}</Typography>
      </Grid>
    </Grid>
  );
  // return (
  //   <Grid container>
  //     {name ? <header>Logged in as {name}</header> : 'Not logged in'}
  //     <label>
  //       Name:
  //       <input
  //         onChange={({ target: { value } }) => setNewName(value)}
  //         value={newName}
  //       />
  //     </label>
  //     <button
  //       onClick={() => {
  //         if (users.includes(newName)) {
  //           console.log('User already logged in');
  //           return;
  //         }
  //         console.log('creating new connection with server');
  //         connect();
  //       }}
  //       disabled={isDisconnected(status) || !newName}
  //     >
  //       Reconnect
  //     </button>
  //     <button
  //       onClick={() => {
  //         console.log('Closing connection with server');
  //         disconnect();
  //       }}
  //       disabled={!isConnected(status)}
  //     >
  //       Disconnect
  //     </button>
  //     <button
  //       type="button"
  //       onClick={() => {
  //         const user = sample(without(users, newName));
  //         if (!user) return;
  //
  //         send(faker.lorem.sentence(), user);
  //         console.log(`Sent random message to ${user}`);
  //       }}
  //       disabled={!isConnected(status)}
  //     >
  //       Send random sentence
  //     </button>
  //     <div style={{ display: 'flex', flexDirection: 'row', columnGap: '1em' }}>
  //       <div style={{ display: 'flex', flexDirection: 'column' }}>
  //         <p>Messages</p>
  //         <ul>
  //           {messages.map((message) => (
  //             <li key={message.contents}>
  //               <span>{message.messenger}</span>
  //               <span> sent </span>
  //               <span>{message.contents}</span>
  //             </li>
  //           ))}
  //         </ul>
  //       </div>
  //     </div>
  //     <MessageList />
  //   </Grid>
  // );
};
