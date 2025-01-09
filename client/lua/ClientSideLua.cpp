//
// Created by Théo Bary on 09/01/2025.
//

#include "ClientSideLua.hpp"

LUA_API int luau_IsKeyPressed(lua_State *L)
{
    int key = lua_tointeger(L, 1);
    bool isKeyPressed = InputManager::GetInstance().isKeyPressed(key);
    lua_pushboolean(L, isKeyPressed);
    return lua_gettop(L);
}

LUA_API int luau_IsKeyJustPressed(lua_State *L)
{
    int key = lua_tointeger(L, 1);
    bool isKeyJustPressed = InputManager::GetInstance().isKeyJustPressed(key);
    lua_pushboolean(L, isKeyJustPressed);
    return lua_gettop(L);
}
