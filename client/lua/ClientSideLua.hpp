//
// Created by Théo Bary on 09/01/2025.
//

#ifndef CLIENTSIDELUA_HPP
    #define CLIENTSIDELUA_HPP

    #include "Lua/lua.hpp"
    #include "input_manager/InputManager.hpp"

LUA_API int luau_IsKeyPressed(lua_State *L);
LUA_API int luau_IsKeyJustPressed(lua_State *L);

#endif //CLIENTSIDELUA_HPP
