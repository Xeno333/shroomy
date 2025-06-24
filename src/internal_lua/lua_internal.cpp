#include <string>
#include "internal_lua.h"
#include "internal_lua_files.h"
#include "luaenv/luaenv.h"


void InternalLua::RunInternalLua(LuaInterface* env) {
    env -> RunString(InternalLuaFiles::MainLua);
    env -> RunString(InternalLuaFiles::Entities);

    env -> RunString("print(\"Initilized internal Lua enviorment\")");
}