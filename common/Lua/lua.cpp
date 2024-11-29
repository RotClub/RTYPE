//
// Created by Théo Bary on 29/11/2024.
//

#include "lua.hpp"

LUA_API int luau_Include(lua_State *L)
{
    const char *filePath = lua_tostring(L, 1);
    if (!filePath) {
        lua_pushstring(L, "Invalid file path provided.");
        lua_error(L);
    }

    try {
        const std::string &script = Engine::GetInstance().GetLuaFileContents(filePath);

        size_t bytecodeSize;
        char *bytecode = luau_compile(script.c_str(), script.size(),  nullptr, &bytecodeSize);

        if (!bytecode) {
            lua_pushstring(L, "Failed to compile the Lua script.");
            lua_error(L);
        }

        const int loadResult = luau_load(L, filePath, bytecode, bytecodeSize, 0);
        free(bytecode);

        if (loadResult != 0) {
            lua_pushfstring(L, "Error loading Lua script: %s", lua_tostring(L, -1));
            lua_error(L);
        }

        if (lua_pcall(L, 0, LUA_MULTRET, 0) != 0) {
            lua_pushfstring(L, "Error executing Lua script: %s", lua_tostring(L, -1));
            lua_error(L);
        }

        return lua_gettop(L);
    } catch (const std::exception& e) {
        lua_pushfstring(L, "Exception: %s", e.what());
        lua_error(L);
    }
}

/* NET LIBRARY */
LUA_API int luau_NetStart(lua_State *L)
{
    const char *packetName = lua_tostring(L, 1);
    const bool reliable = lua_toboolean(L, 2);
    std::cout << "Starting network packet: " << packetName << " (reliable: " << reliable << ")" << std::endl;
    return lua_gettop(L);
}
/* NET LIBRARY */

static void luau_ExposeGlobalFunction(lua_State *L, const lua_CFunction func, const char *name)
{
    lua_pushcfunction(L, func, name);
    lua_setglobal(L, name);
}

static void luau_ExposeFunctionsAsLibrary(lua_State *L, const luaL_Reg *functions, const char *name)
{
    lua_newtable(L);
    luaL_register(L, name, functions);
    lua_newtable(L);
    lua_pushstring(L, "__newindex");
    lua_pushcfunction(L, [](lua_State *Lm) -> int {
        luaL_error(Lm, "attempt to modify a read-only table");
        return 0;
    }, "name");
    lua_settable(L, -3);

    lua_pushstring(L, "__metatable");
    lua_pushstring(L, "protected");
    lua_settable(L, -3);

    lua_setmetatable(L, -2);

    lua_setglobal(L, name);
}

void luau_ExposeFunctions(lua_State *L)
{
    luau_ExposeGlobalFunction(L, luau_Include, "include");

    /* NET LIBRARY */
    constexpr luaL_Reg netLibrary[] = {
        {"Start", luau_NetStart},
        {nullptr, nullptr}
    };
    luau_ExposeFunctionsAsLibrary(L, netLibrary, "net");
    /* NET LIBRARY */
}
