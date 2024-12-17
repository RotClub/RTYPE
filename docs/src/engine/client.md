# Client

The client runs the game and is responsible for rendering the game state and sending the input to the server.

The client is written in C++ and uses the Raylib library to render the game.

The client can be run with the following command:
```bash
./r-type_client
```

The client can be run with the following options:
- `--ip` to specify the server IP address
- `--port` to specify the server port

The client can use two types of connection: TCP and UDP.

## TCP Connection

The TCP connection is used to do the initial handshake between the client and the server.
The client will send a message to the server to ask to join the game.
The server will then send a message to the client to tell him if he can join the game or not.
If the client can join the game, the server will send the game state to the client.

It is also used ...

## UDP Connection

The UDP connection is used to send the input to the server.
The client will send the input to the server at a fixed rate.
The server will then update the game state and send it to the client.

It is also used ...
