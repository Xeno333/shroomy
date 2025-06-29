# Shroomy

Shroomy is a 2D game eninge, for Lua games with a built in modding interface. It is built on SDL for rendering and achives perfromance in Lua by using a mix of C++ and LuaJIT.

Shroomy is a fully single threaded engine, including rendering client/server and all Lua code.

**Note**: This project is a work in progress with some things that havent been fully implimented into the C++/Lua interface.


## Features

- Lua defined games and mods.
- Client/Server interface for multiplayer games through lua interface.
- Content assets of `.png` and `.wav` for image and audio respectively.
- Game defined controls through configuration file.
- Basic Lua defined entity system with collision check and rendering methods.
- A Lua API for using engine features (see docs).


## Build

Currently Shroomy only works on *nix systems, this is planned to be expanded to Windows in the near future.

The following libraries must be installed: `libsdl2`, `libsdl2-mixer`, `libsdl2-image`.
LuaJIT library must also be installed, for *nix systems with git and make the `init.sh` script is provided for bash to do this.

The engine is built with the included cmake script:

```
    cmake .
    make
```

The following cmake flags are also provided:

```
    -DDEBUG=TRUE            Build debug version.
    -DOPTIMIZED_DEBUG=TRUE  Build debug with optimizations enabled.
    -DDEBUG_LUA=TRUE        Builds with debug flag set in Lua.
```