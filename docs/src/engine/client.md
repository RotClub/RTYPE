# Client

The client runs the game and is responsible for rendering the game state and sending the input to the server.

The client is written in C++ and uses the Raylib library to render the game.

The client can be run with the following options:
- `--ip` to specify the server IP address
- `--port` to specify the server port
- `--colorblindness` to enable colorblindness mode

It can be run with the following command:
```bash
./r-type_client --ip [ip_address] --port [port] --colorblindness [none|protanopia|deuteranopia|tritanopia]
```
