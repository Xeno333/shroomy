#ifndef LUAINTERFACE
    #include <lua.hpp>

    class LuaInterface {
        private:
            lua_State* LuaState = nullptr;

        public:
            lua_State* GetTheState();

            bool RunFile(std::string filename);
            bool RunString(std::string lua_string);
            bool IsValid();

            LuaInterface() {
                LuaState = luaL_newstate();
                if (LuaState != nullptr) {
                    luaL_openlibs(LuaState);
                }
            };
            ~LuaInterface() {
                if (LuaState != nullptr) {
                    lua_close(LuaState);
                }
            };
    };

    #define LUAINTERFACE true
#endif