//
// Created by Théo Bary on 09/01/2025.
//

#include "ClientSideLua.hpp"

LUA_API int luau_IsKeyPressed(lua_State *L)
{
    int key = lua_tointeger(L, 1);
    lua_remove(L, 1);
    bool isKeyPressed = IsKeyDown(key);
    lua_pushboolean(L, isKeyPressed);
    return lua_gettop(L);
}

LUA_API int luau_IsKeyJustPressed(lua_State *L)
{
    int key = lua_tointeger(L, 1);
    lua_remove(L, 1);
    bool isKeyJustPressed = IsKeyPressed(key);
    lua_pushboolean(L, isKeyJustPressed);
    return lua_gettop(L);
}

LUA_API int luau_EnableFpsCounter(lua_State *L)
{
    return lua_gettop(L);
}
