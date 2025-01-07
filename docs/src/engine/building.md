# Building

The engine is a shared library that is used by the server and the client.
The server and the client are two separate executables that use the engine library.
You can build all the project with the following command:

```bash
./build.sh release
```

This command will build the engine, the server and the client in release mode.
The executables will be at the root of the project.

You can also build the engine, the server and the client separately with the following commands:

```bash
./build.sh release engine
./build.sh release server
./build.sh release client
```

This will build the engine, the server or the client in release mode.

You can also build the project in debug mode with the following command:

```bash
./build.sh debug
```

or

```bash
./build.sh debug engine
./build.sh debug server
./build.sh debug client
```

This will build the engine, the server or the client in debug mode.
