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
    #include <format>
    #include "lua.h"
    #include "luaconf.h"
    #include "lualib.h"
    #include "luacode.h"
    #include <queue>
    #include <filesystem>
    #include "../GameInfo/GameInfo.hpp"

    void luau_ExposeFunctions(lua_State *L);
    void luau_ExposeGameInfoTable(lua_State *L, const GameInfo *info);
    void luau_ExposeConstants(lua_State *L, Types::VMState state);

    LUA_API int luau_Import(lua_State *L);

    /* NET LIBRARY */
    LUA_API int luau_NetCreatePacket(lua_State *L);
    LUA_API int luau_NetStart(lua_State *L);
    LUA_API int luau_NetSendToServer(lua_State *L);
    LUA_API int luau_NetSendToClient(lua_State *L);
    LUA_API int luau_NetBroadcast(lua_State *L);

    /* READ/WRITE OPERATIONS */ // ALL OPERATIONS HERE MUST HAVE A READ AND WRITE PAIR
    LUA_API int luau_NetWriteString(lua_State *L);
    LUA_API int luau_NetReadString(lua_State *L);
    /* READ/WRITE OPERATIONS */

    /* NET LIBRARY */


    /* HOOK LIBRARY */
    LUA_API int luau_HookAdd(lua_State *L);
    LUA_API int luau_HookRemove(lua_State *L);
    LUA_API int luau_HookCall(lua_State *L);
    /* HOOK LIBRARY */

#endif //LUA_HPP
