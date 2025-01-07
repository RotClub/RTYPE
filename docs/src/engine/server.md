# Server

The server is responsible of managing the game state and the communication between the clients and the game.
When the server is started, it will start the game engine and the connection manager.
It run on the port "25777" by default but it can be changed with the `--port` option.
The server can use two types of connection: TCP and UDP.

The command to run the server is:
```bash
./r-type_server
```
or with the port option:
```bash
./r-type_server --port 12345
```

## TCP Connection

The TCP connection is used to do the initial handshake between the client and the server.
The client will send a message to the server to ask to join the game.
The server will then send a message to the client to tell him if he can join the game or not.
If the client can join the game, the server will send the game state to the client.

It is also used ...

## UDP Connection

The UDP connection is used to send the game state to the clients.
The server will send the game state to all the clients at a fixed rate.
The client will then update the game state and render it and send the input to the server.

It is also used ...