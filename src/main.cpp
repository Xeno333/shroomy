#include <iostream>
#include <filesystem>
#include <chrono>
#include "shroomy.h"
#include "interface/interface.h"
#include "luaenv/luaenv.h"
#include "luaenv/lua_functions.h"
#include "main.h"
#include "signal.h"

namespace Main {
    bool Running = true;

    std::string Path = ".";
    Interface Window(true);
}


int main(int argc, char** argv) {
    std::cout << "Starting " << Shroomy::Version << " Copyright (c) 2025 Xeno333..." << std::endl;

    if (!Main::Window.IsValid()){
        std::cout << "Error: Video failed to initialize!" << std::endl;
        return 1;
    }

    if (argc > 1) {
        Main::Path = std::string(argv[1]) + DIR_SEP_CHAR;
        std::cout << "Loading in path " << Main::Path << std::endl;
    }

    Signals::RegisterSignalHandelers();


    // LuaMainInstance
    LuaInterface LuaMainInstance;
    if (!LuaMainInstance.IsValid()) {
        std::cout << "Error: LuaJIT failed to initialize!" << std::endl;
        return 1;
    }
    std::cout << "Initilized LuaJIT" << std::endl;

    LuaAPI::LoadLuaAPI(&LuaMainInstance);
    LuaMainInstance.RunString("print(\"Initilized internal Lua enviorment\")");

    // Run lua code
    LuaMainInstance.RunFile(Main::Path + "init.lua");


    // Load mods
    std::string mods = Main::Path + "mods" + DIR_SEP_CHAR;
    if (std::filesystem::is_directory(mods)) {
        for (const auto &dir : std::filesystem::directory_iterator(mods)) {
            if (dir.is_directory()) {
                std::cout << "[ModLoader] Loading mod found at" << dir.path().generic_string() << "`..." << std::endl;
                LuaMainInstance.RunFile(dir.path().string() + DIR_SEP_CHAR + "init.lua");
            }
        }
    }


    if (LuaMainInstance.RunString("assert(GameLoop)")) {
        auto last_tick = std::chrono::high_resolution_clock::now();

        bool Do_OnGameTick = false;
        if (LuaMainInstance.RunString("assert(OnGameTick)"))
            Do_OnGameTick = true;

        while (Main::Running) {
            Main::Window.GetEvents();
            if (Main::Window.IsKeyPressed(SDL_SCANCODE_ESCAPE))
                break;

            Main::Window.Clear();

            if (Do_OnGameTick) {
                auto time_now = std::chrono::high_resolution_clock::now();
                if ((std::chrono::duration_cast<std::chrono::milliseconds>(time_now - last_tick)).count() > SHROOMY_TICK_RATE_MS) {
                    if (!LuaMainInstance.RunString("OnGameTick()")) {
                        break;
                    }

                    last_tick = time_now;
                }
            }

            if (!LuaMainInstance.RunString("GameLoop()")) {
                break;
            }

            Main::Window.Render();
            SDL_Delay(17);
        }
    } else {
        std::cout << "[EngineLuaEnv] GameLoop is nil!" << std::endl;
    }

    return 0;
}