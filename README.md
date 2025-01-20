# R-TYPE

## Information:

The purpose of this project is to create a game engine and a game based on the R-TYPE license. The game engine is a 2D game engine that uses a Node System to manage the game objects, is inspired by [Godot](https://docs.godotengine.org/en/stable/getting_started/step_by_step/nodes_and_scenes.html). The engine is written in C++ and the games are written in Luau, like Roblox.

This project uses a mirroring system for repository management. This ensures synchronization between multiple instances of the source code, promoting its availability and long-term sustainability.

You can find it [here](https://github.com/RotClub/RTYPE)

## Documentation:

[R-TYPE documentation](https://rotclub.github.io/RTYPE/)
[R-TYPE RFC](https://github.com/RotClub/RTYPE/tree/main/docs/rfc.txt)

## Build:

This project works on Linux, MacOS and Windows and is built with CMake.
Before building the project, you need to pull the submodules with the following command:

```bash
git submodule update --init --recursive
```

Then you can build all the project with the following command:

```bash
./build.sh release
```

This command will build the engine, the server and the client in release mode.
If you want more information about the build script or build the project in debug mode, you can go to the [build documentation](https://rotclub.github.io/RTYPE/engine/building.html)

## Contributors:

- __Théo Bary__
- __Yanis Moyon__
- __Léo Maurel__
- __Paul Arbez__
- __Jacques Sapin__
