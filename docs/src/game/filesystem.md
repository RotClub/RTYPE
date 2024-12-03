# Games Filesystem

Each game is put in the `games/` folder. The directory architecture is strict and must be followed as such:

*Files/Folders in **bold** are required for the project to be valid.*

- **manifest.json**
- assets/
- **lua/**
  - **index.lua** *entrypoint for every game*

## manifest.json

The manifest file is used for the server to know the presence of a valid game and retrieve basic information to send to the players on join attempts.

```json
{
    "name": "string",
    "description": "string",
    "max_players": 4,
    "authors": ["string", "string"],
    "version": 1.0
}
```
