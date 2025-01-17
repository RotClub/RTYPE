# Building

The engine is a shared library that is used by the server and the client.
The server and the client are two separate executables that use the engine library.
Before building the project, you need to pull the submodules with the following command:

```bash
git submodule update --init --recursive
```

Then you can build all the project with the following command:

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

You can also build the project in profile mode with the following command:

```bash
./build.sh profile
```

or

```bash
./build.sh profile engine
./build.sh profile server
./build.sh profile client
```

This mode is used tu use a profiler to analyze the performance of the engine and the game.
