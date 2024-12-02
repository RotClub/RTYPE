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
LUA_API int luau_NetCreatePacket(lua_State *L)
{
    const char *packetName = lua_tostring(L, 1);
    if (!packetName) {
        lua_pushstring(L, "Invalid packet name provided.");
        lua_error(L);
    }
    if (Engine::GetInstance().hasPacket(packetName)) {
        lua_pushstring(L, "Packet already exists.");
        lua_error(L);
    }
    const bool reliable = lua_toboolean(L, 2);
    Engine::GetInstance().addPacket(packetName, reliable);
    return lua_gettop(L);
}

LUA_API int luau_NetStart(lua_State *L)
{
    const char *packetName = lua_tostring(L, 1);
    if (!packetName) {
        lua_pushstring(L, "Invalid packet name provided.");
        lua_error(L);
    }
    if (!Engine::GetInstance().hasPacket(packetName)) {
        lua_pushstring(L, std::format("Packet \"{}\" has not been initialized. Try calling net.CreatePacket first.", packetName).c_str());
        lua_error(L);
    }
    // TODO: add the real deal here.
    return lua_gettop(L);
}

LUA_API int luau_NetSendToServer(lua_State *L)
{
    const char *packetName = lua_tostring(L, 1);
    std::cout << "Sending packet to server: " << packetName << std::endl;
    return lua_gettop(L);
}

LUA_API int luau_NetSendToClient(lua_State *L)
{
    const char *packetName = lua_tostring(L, 1);
    std::cout << "Sending packet to client: " << packetName << std::endl;
    return lua_gettop(L);
}
LUA_API int luau_NetBroadcast(lua_State *L)
{
    const char *packetName = lua_tostring(L, 1);
    std::cout << "Broadcasting packet: " << packetName << std::endl;
    return lua_gettop(L);
}

/* READ/WRITE OPERATIONS */ // ALL OPERATIONS HERE MUST HAVE A READ AND WRITE PAIR
LUA_API int luau_NetWriteString(lua_State *L)
{
    const char *str = lua_tostring(L, 1);
    if (!str) {
        lua_pushstring(L, "Invalid string provided.");
        lua_error(L);
    }
    // TODO: add the real deal here.
    return lua_gettop(L);
}

LUA_API int luau_NetReadString(lua_State *L)
{
    // TODO: add the real deal here.
    return lua_gettop(L);
}
/* READ/WRITE OPERATIONS */

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
        {"CreatePacket", luau_NetCreatePacket},
        {"Start", luau_NetStart},
        {"SendToServer", luau_NetSendToServer},
        {"SendToClient", luau_NetSendToClient},
        {"Broadcast", luau_NetBroadcast},
        {"WriteString", luau_NetWriteString},
        {"ReadString", luau_NetReadString},
        {nullptr, nullptr}
    };
    luau_ExposeFunctionsAsLibrary(L, netLibrary, "net");
    /* NET LIBRARY */
}
