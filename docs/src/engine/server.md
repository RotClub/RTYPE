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
