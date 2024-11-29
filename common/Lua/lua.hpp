//
// Created by Théo Bary on 29/11/2024.
//

#ifndef LUA_HPP
    #define LUA_HPP

    #ifndef ENGINE_HPP_
        #include "Engine.hpp"
    #endif

    #include <iostream>
    #include <fstream>
    #include <sstream>
    #include "lua.h"
    #include "luaconf.h"
    #include "lualib.h"
    #include "luacode.h"
    #include <queue>
    #include <filesystem>

    void luau_ExposeFunctions(lua_State *L);

    LUA_API int luau_Import(lua_State *L);

    /* NET LIBRARY */
    LUA_API int luau_NetStart(lua_State *L);
    /* NET LIBRARY */

#endif //LUA_HPP
