
# G10 Engine
NOTE: This is a hobby project. Please do not use in commercial code until such time as this warning is removed. Thank you.

## Intro

Howdy! I'm Jake. This is my project, the G10 game engine. 

You can use the G10 game engine to create 3D games. Examples of other game engines include, Unreal®, Unity®, or Source®. These are often very large projects that manage gigabytes of assets on the filesystem and require auxiliary programs, like map editors, to be useful. This project is no exception.

There are three main components of G10

> [G10 (The Engine)](https://github.com/Jacob-C-Smith/G10-Vulkan)
>
> [GPort (The Import/Exporter)](https://github.com/Jacob-C-Smith/GPort)
>
> [G10 Schema (JSON Schema)](https://github.com/Jacob-C-Smith/G10-Schema)

G10 Schema is a set of JSON schemas descring valid nontemporal game data. In other words, anything stored on the disk, such as scenes, game and server configuration, models, textures, animation, shaders, etc. 

## A primer on G10
##### Making an application with G10, generating assets
To start working with the engine, download the latest build of G10 for Visual C. Place the DLL files where you plan to build your executable. Be sure to set up your compiler options correctly. Create a JSON file, and use the [instance schema](https://github.com/Jacob-C-Smith/G10-Schema) to 
