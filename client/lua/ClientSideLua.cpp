//
// Created by Théo Bary on 09/01/2025.
//

#include "ClientSideLua.hpp"

#include <Client.hpp>
#include <config/Config.hpp>

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
    Config::GetInstance().setFpsCounter(true);
    return lua_gettop(L);
}

LUA_API int luau_DisableFpsCounter(lua_State *L)
{
    Config::GetInstance().setFpsCounter(false);
    return lua_gettop(L);
}

LUA_API int luau_DisableColorBlindnessShader(lua_State *L)
{
    Config::GetInstance().setColorBlindnessMode(Config::ColorBlindnessMode::NONE);
    return lua_gettop(L);
}

LUA_API int luau_CycleColorBlindnessShader(lua_State *L)
{
    Config::GetInstance().iterateColorBlindnessMode();
    return lua_gettop(L);
}

LUA_API int luau_WindowHeight(lua_State *L)
{
    Client &client = Client::GetInstance();
    int height = client.getGameSize().x;
    lua_pushinteger(L, height);
    return lua_gettop(L);
}

LUA_API int luau_WindowWidth(lua_State *L)
{
    Client &client = Client::GetInstance();
    int width = client.getGameSize().y;
    lua_pushinteger(L, width);
    return lua_gettop(L);
}
