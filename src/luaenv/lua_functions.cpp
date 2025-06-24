#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "lua_functions.h"
#include "luaenv.h"
#include "main.h"
#include "interface/interface.h"



static int shroomy_say(lua_State* L) {
    if (lua_isstring(L, 1))
        std::cout << "[ShroomySay] " << lua_tostring(L, 1) << std::endl;
    return 0;
}

static int load_wav(lua_State* L) {
    if (lua_isstring(L, 1) && lua_isstring(L, 2)) {
        lua_pushboolean(L, Main::Window.LoadWav(lua_tostring(L, 1), lua_tostring(L, 2)));
    } else {
        lua_pushnil(L);
    }

    return 1;
}

static int play_sound(lua_State* L) {
    if (lua_isstring(L, 1), lua_isnumber(L, 2)) {
        lua_pushboolean(L, Main::Window.PlaySound(lua_tostring(L, 1), (int)lua_tointeger(L, 2)));
    } else {
        lua_pushnil(L);
    }

    return 1;
}

static int load_texture(lua_State* L) {
    if (lua_isstring(L, 1) && lua_isstring(L, 2)) {
        lua_pushboolean(L, Main::Window.LoadTexture(lua_tostring(L, 1), lua_tostring(L, 2)));
    } else {
        lua_pushnil(L);
    }

    return 1;
}

static int render_texture(lua_State* L) {
    if (lua_isstring(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4) && lua_isnumber(L, 5)) {
        SDL_Rect rect = {
            (int)lua_tointeger(L, 2), (int)lua_tointeger(L, 3),
            (int)lua_tointeger(L, 4), (int)lua_tointeger(L, 5)
        };

        lua_pushboolean(L, Main::Window.RenderTexture(lua_tostring(L, 1), &rect));
    } else {
        lua_pushnil(L);
    }

    return 1;
}

static std::map<std::string, SDL_Scancode> KeyBinds;

static int is_key_pressed(lua_State* L) {
    if (lua_isstring(L, 1)) {
        std::string keybind = lua_tostring(L, 1);
        if (KeyBinds.contains(keybind)) {
            lua_pushboolean(L, Main::Window.IsKeyPressed(KeyBinds[keybind]));
            return 1;
        }
    }

    lua_pushnil(L);
    return 1;
}

static int set_window_size(lua_State* L) {
    if (lua_isnumber(L, 1) && lua_isnumber(L, 2)) {
        Main::Window.ResizeWindow((int)lua_tointeger(L, 1), (int)lua_tointeger(L, 2));

        lua_pushboolean(L, true);
        return 1;
    }
    
    lua_pushboolean(L, false);
    return 1;
}

static int set_tick(lua_State* L) {
    if (lua_isnumber(L, 1)) {
        Main::tick_time_ms = (int)lua_tointeger(L, 1);

        lua_pushboolean(L, true);
        return 1;
    }
    
    lua_pushboolean(L, false);
    return 1;
}





// Registration



void RegisterFunction(lua_State* lua_state, std::string name, int (*func)(lua_State*)) {
    lua_getglobal(lua_state, "shroomy");

    lua_pushcfunction(lua_state, func);
    lua_setfield(lua_state, -2, name.c_str());

    lua_pop(lua_state, 1);
}

void LuaAPI::LoadLuaAPI(LuaInterface *lua_instance) {
    lua_State* lua_state = lua_instance -> GetTheState();
    lua_newtable(lua_state);
    lua_setglobal(lua_state, "shroomy");

    #ifdef SHROOMY_DEBUG
        lua_getglobal(lua_state, "shroomy");

        lua_pushboolean(lua_state, true);
        lua_setfield(lua_state, -2, "IS_DEBUG");

        lua_pop(lua_state, 1);
    #endif

    std::ifstream keyconf(Main::Path + "keyconf.scf");
    if (keyconf.is_open()) {
        std::string line_in;
        while (std::getline(keyconf, line_in)) {
            std::stringstream parts_in(line_in);

            std::string key;
            if (std::getline(parts_in, key, ' ')) {
                std::string scancode;
                if (std::getline(parts_in, scancode)) {
                    KeyBinds[key] = (SDL_Scancode)std::stoi(scancode);
                } else {
                    std::cout << "Warning: missing scancode for keybind '" << key << "'" << std::endl;
                }
            }
        }

        keyconf.close();
    } else {
        KeyBinds["FORWARD"] = SDL_SCANCODE_D;
        KeyBinds["BACKWARD"] = SDL_SCANCODE_A;
        KeyBinds["UP"] = SDL_SCANCODE_W;
        KeyBinds["DOWN"] = SDL_SCANCODE_S;
        KeyBinds["SPACE"] = SDL_SCANCODE_SPACE;
    }

    RegisterFunction(lua_state, "set_tick", &set_tick);
    RegisterFunction(lua_state, "shroomy_say", &shroomy_say);
    RegisterFunction(lua_state, "load_wav", &load_wav);
    RegisterFunction(lua_state, "play_sound", &play_sound);
    RegisterFunction(lua_state, "load_texture", &load_texture);
    RegisterFunction(lua_state, "render_texture", &render_texture);
    RegisterFunction(lua_state, "is_key_pressed", &is_key_pressed);
    RegisterFunction(lua_state, "set_window_size", &set_window_size);
}