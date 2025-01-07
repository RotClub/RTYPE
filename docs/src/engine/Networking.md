# Networking

[RFC](https://github.com/RotClub/RTYPE/tree/main/docs/rfc.txt)

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