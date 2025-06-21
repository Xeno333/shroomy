#include <iostream>
#include <lua.hpp>
#include "luaenv.h"


bool LuaInterface::RunFile(std::string filename) {
    if (luaL_loadfile(LuaState, filename.c_str()) != 0) {
        std::cout << "LuaEnv: Could not load `" << filename << "` Error: " << lua_tostring(LuaState, -1) << std::endl;
        lua_pop(LuaState, 1);
        return false;
    }
    if (lua_pcall(LuaState, 0, 0, 0) != 0) {
        std::cout << "LuaEnv: Error in `" << filename << "` Error: " << lua_tostring(LuaState, -1) << std::endl;
        lua_pop(LuaState, 1);
        return false;
    }

    return true;
}


lua_State* LuaInterface::GetTheState() {
    if (!IsValid())
        throw;

    return LuaState;
}


bool LuaInterface::RunString(std::string lua_string) {
    if (luaL_dostring(LuaState, lua_string.c_str()) != LUA_OK) {
        std::cout << "LuaEnv Error: " << lua_tostring(LuaState, -1) << std::endl;
        lua_pop(LuaState, 1);
        return false;
    }

    return true;
}

bool LuaInterface::IsValid() {
    if (LuaState == nullptr) {
        return false;
    }

    return true;
}