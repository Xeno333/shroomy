#include <iostream>
#include <filesystem>
#include <chrono>
#include "shroomy.h"
#include "interface/interface.h"
#include "luaenv/luaenv.h"
#include "luaenv/lua_functions.h"
#include "main.h"
#include "shroomysignal.h"
#include "internal_lua/internal_lua.h"
#include "network/network.h"

namespace Main {
    bool Running = true;

    std::string Path = ".";
    int tick_time_ms = 10;
    Interface Window(true);
    bool needs_window = true;

    Server server;
    Client client;
    Network::NetworkState NetworkingConf = Network::NONE;
}


int main(int argc, char** argv) {
    ShroomySignals::RegisterSignalHandelers();

    std::cout << "Starting " << Shroomy::Version << " Copyright (c) 2025 Xeno333..." << std::endl;

    if (argc > 1) {
        Main::Path = std::string(argv[1]) + DIR_SEP_CHAR;
        std::cout << "Loading in path " << Main::Path << std::endl;

        for (int i = 1; i < argc; i++) {
            std::string param = std::string(argv[i]);
            if (param == "S") {
                if (argc > i+2) {
                    Main::server.Configure(std::stoi(argv[i+1]), argv[i+2]);
                    i = i + 2;
                } else {
                    std::cout << "Error: Server requires usage: shroomy <Path> S <Port> <IP>" << std::endl;
                    return 1;
                }

                while (Main::Running && !Main::server.Init());
                Main::NetworkingConf = Network::SERVER;
            }
            else if (param == "C") {
                if (argc > i+2) {
                    Main::client.Configure(std::stoi(argv[i+1]), argv[i+2]);
                    i = i + 2;
                } else {
                    std::cout << "Error: Server requires usage: shroomy <Path> C <Port> <IP>" << std::endl;
                    return 1;
                }

                while (Main::Running && !Main::client.Init());
                Main::NetworkingConf = Network::CLIENT;
            }
            else if (param == "help") {
                std::cout << "shroomy <Path> <S/C <Port> <IP>>" << std::endl;
                return 0;
            }
            else if (param == "headless") {
                Main::needs_window = false;
                std::cout << "WARNING: 'headless' stops render loops and does not take SDL input";
                Main::Window.~Interface();
            }
        }
    }

    if (!Main::Window.IsValid() && Main::needs_window){
        std::cout << "Error: Video failed to initialize!" << std::endl;
        return 1;
    }


    // LuaMainInstance
    LuaInterface LuaMainInstance;
    if (!LuaMainInstance.IsValid()) {
        std::cout << "Error: LuaJIT failed to initialize!" << std::endl;
        return 1;
    }
    std::cout << "Initilized LuaJIT" << std::endl;

    LuaAPI::LoadLuaAPI(&LuaMainInstance);

    InternalLua::RunInternalLua(&LuaMainInstance);

    // Run lua code
    if (!LuaMainInstance.RunFile(Main::Path + "init.lua")) {
        std::cout << "[EngineLuaEnv] Could not load main script!" << std::endl;
        return 1;
    }


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


    bool Do_OnGameTick = LuaMainInstance.RunString("assert(OnGameTick)", false);
    bool Do_RenderLoop = LuaMainInstance.RunString("assert(RenderLoop)", false);
    bool Do_Server = LuaMainInstance.RunString("assert(Server)", false);
    bool Do_Client = LuaMainInstance.RunString("assert(Client)", false);

    if ((Do_OnGameTick || Do_RenderLoop) && (Main::NetworkingConf != Network::SERVER || (Do_Server && Main::NetworkingConf == Network::SERVER)) && (Main::NetworkingConf != Network::CLIENT || (Do_Client && Main::NetworkingConf == Network::CLIENT))) {
        auto last_tick = std::chrono::high_resolution_clock::now();
        int passed_time = 0;

        // FPS debug
        #ifdef SHROOMY_DEBUG
            auto last_frame = std::chrono::high_resolution_clock::now();
            auto current_frame = std::chrono::high_resolution_clock::now();
            auto last_debug_print = std::chrono::high_resolution_clock::now();
            bool print_debug = false;
            bool print_debug_game_tick = false;
        #endif

        bool window_is_valid = Main::needs_window;
        while (Main::Running) {
            // Controls
            if (window_is_valid) {
                Main::Window.GetEvents();
                if (Main::Window.IsKeyPressed(SDL_SCANCODE_ESCAPE) && Main::Window.IsKeyPressed(SDL_SCANCODE_LCTRL))
                    break;

                #ifdef SHROOMY_DEBUG
                    current_frame = std::chrono::high_resolution_clock::now();
                    if ((std::chrono::duration_cast<std::chrono::seconds>(current_frame - last_debug_print)).count() >= 1) {
                        last_debug_print = current_frame;
                        print_debug = true;
                        print_debug_game_tick = true;
                    }
                #endif

                Main::Window.Clear();
            }

            bool tick_now = false;
            if (Do_OnGameTick || Main::NetworkingConf != Network::NONE) {
                auto time_now = std::chrono::high_resolution_clock::now();
                passed_time = (std::chrono::duration_cast<std::chrono::milliseconds>(time_now - last_tick)).count();
                if (passed_time > Main::tick_time_ms) {
                    last_tick = time_now;
                    tick_now = true;
                }
            }

            switch (Main::NetworkingConf) {
                case Network::CLIENT:
                    if (!Main::client.IsValid()) {
                        if (tick_now)
                            Main::client.Init();
                        continue;
                    }
                    break;
                case Network::SERVER:
                    if (!Main::server.IsValid()) {
                        if (tick_now)
                            Main::server.Init();
                        continue;
                    }
                    break;
                default:
                    break;
            }

            // Tick and server
            if (tick_now) {
                // Server
                switch (Main::NetworkingConf) {
                    case Network::CLIENT:
                        if (!Main::client.DoClient(&LuaMainInstance)) {
                            continue;
                        }
                        break;
                    case Network::SERVER:
                        if (!Main::server.Serve(&LuaMainInstance)) {
                            continue;
                        }
                        break;
                    default:
                        break;
                }

                // Tick
                if (Do_OnGameTick) {
                    #ifdef SHROOMY_DEBUG
                        auto start_tick_time = std::chrono::high_resolution_clock::now();
                    #endif

                    if (!LuaMainInstance.RunString(std::string("OnGameTick(") + std::to_string(passed_time) + ")")) {
                        break;
                    }

                    #ifdef SHROOMY_DEBUG
                        if (print_debug_game_tick) {
                            std::cout << "[DEBUG] OnGameTick took: " << (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_tick_time)).count() << " us." << std::endl;
                            print_debug_game_tick = false;
                        }
                    #endif
                }
            }


            // Render
            if (window_is_valid) {
                if (Do_RenderLoop) {
                    #ifdef SHROOMY_DEBUG
                        auto start_render_time = std::chrono::high_resolution_clock::now();
                    #endif

                    if (!LuaMainInstance.RunString("RenderLoop()")) {
                        break;
                    }

                    #ifdef SHROOMY_DEBUG
                        if (print_debug)
                            std::cout << "[DEBUG] RenderLoop took: " << (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_render_time)).count() << " us. to render scene." << std::endl;
                    #endif
                }

                Main::Window.Render();


                // FPS debug
                #ifdef SHROOMY_DEBUG
                    if (print_debug) {
                        int64_t t = (std::chrono::duration_cast<std::chrono::microseconds>(current_frame - last_frame)).count();
                        std::cout << "[DEBUG] FPS: " << 1000000 / (t) << std::endl;

                        print_debug = false;
                    }

                    last_frame = current_frame;
                #endif
            }
        }
    } else {
        std::cout << "[EngineLuaEnv] Neither 'OnGameTick' or 'RenderLoop' are defined, OR attempting to run server/client without interface!" << std::endl;
        std::cout << "[EngineLuaEnv] Client is " << Do_Client << ", Server is " << Do_Server << ", OnGameTick is " << Do_OnGameTick << ", RenderLoop is " << Do_RenderLoop << std::endl;
        return 1;
    }

    return 0;
}