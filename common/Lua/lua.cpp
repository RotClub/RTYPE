//
// Created by Théo Bary on 29/11/2024.
//

#include "lua.hpp"

#include <Nodes/Node2D/Node2D.hpp>
#include <Nodes/Node2D/Sprite2D/Sprite2D.hpp>

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

        Engine::GetInstance().Log(Engine::LogLevel::DEBUG, std::format("Successfully loaded file: {}", filePath));
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

/* NODE LIBRARY */
void luau_ExposeRootNode(lua_State *L)
{
    Node *root = Engine::GetInstance().root;
    *(Node**)lua_newuserdata(L, sizeof(Node*)) = root;
    luaL_getmetatable(L, "NodeMetaTable");
    lua_setmetatable(L, -2);
    lua_setglobal(L, "root");
}

LUA_API int lua_gcNode(lua_State* L)
{
    Node* node = *(Node**)luaL_checkudata(L, 1, "NodeMetaTable");
    delete node;
    return 0;
}

LUA_API int lua_gcNode2D(lua_State* L)
{
    Node2D* node2D = *(Node2D**)luaL_checkudata(L, 1, "Node2DMetaTable");
    delete node2D;
    return 0;
}

LUA_API int lua_gcSprite2D(lua_State* L)
{
    Sprite2D* sprite2D = *(Sprite2D**)luaL_checkudata(L, 1, "Sprite2DMetaTable");
    delete sprite2D;
    return 0;
}

void luau_ExposeFunctionsAsMetatable(lua_State *L, const luaL_Reg *functions, const char *name, const char *parent) {
    // Get or create the metatable
    if (luaL_getmetatable(L, name) == LUA_TNIL) {
        lua_pop(L, 1); // Pop nil
        luaL_newmetatable(L, name);
    }

    // Add functions to the metatable
    for (const luaL_Reg *func = functions; func->name; func++) {
        lua_pushcfunction(L, func->func, name);
        lua_setfield(L, -2, func->name);
    }

    // Set parent metatable for inheritance
    if (parent) {
        if (luaL_getmetatable(L, parent) != LUA_TNIL) {
            lua_setfield(L, -2, "__index");
        } else {
            lua_pop(L, 1);
            luaL_error(L, "Parent metatable '%s' not found", parent);
        }
    } else {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
    }

    // Pop the metatable
    lua_pop(L, 1);
}


LUA_API int luau_NodeGetName(lua_State *L)
{
    Node* node = *(Node**)luaL_checkudata(L, 1, "NodeMetaTable");
    lua_pushstring(L, node->name.c_str());
    return 1;
}

LUA_API int luau_NodeSetName(lua_State *L)
{
    Node* node = *(Node**)luaL_checkudata(L, 1, "NodeMetaTable");
    const char *name = luaL_checkstring(L, 2);
    node->name = name;
    return 0;
}

LUA_API int luau_NodeGetChildren(lua_State *L)
{
    Node* node = *(Node**)luaL_checkudata(L, 1, "NodeMetaTable");
    std::vector<Node> &children = node->children;
    lua_newtable(L);
    for (size_t i = 0; i < children.size(); i++) {
        *(Node**)lua_newuserdata(L, sizeof(Node*)) = &children[i];
        luaL_getmetatable(L, "NodeMetaTable");
        lua_setmetatable(L, -2);
        lua_rawseti(L, -2, i + 1);
    }
    return 1;
}

LUA_API int luau_NodeGetChild(lua_State* L) {
    Node* node = *(Node**)luaL_checkudata(L, 1, "NodeMetaTable");
    const char* childName = luaL_checkstring(L, 2);
    int find = 0;

    std::vector<Node> &children = node->children;
    for (auto &child : children) {
        if (child.name == childName) {
            *(Node**)lua_newuserdata(L, sizeof(Node*)) = &child;
            luaL_getmetatable(L, "NodeMetaTable");
            lua_setmetatable(L, -2);
            find = 1;
        }
    }
    if (!find)
        lua_pushnil(L);
    return 1;
}

LUA_API int luau_NodeAddChild(lua_State *L)
{
    Node* node = *(Node**)luaL_checkudata(L, 1, "NodeMetaTable");
    const std::string type = luaL_checkstring(L, 2);
    const char *name = luaL_checkstring(L, 3);
    if (type == "Node") {
        auto child = new Node(name);
        *(Node**)lua_newuserdata(L, sizeof(Node*)) = child;
        luaL_getmetatable(L, "NodeMetaTable");
        lua_setmetatable(L, -2);
        node->children.push_back(*child);
    }
    else if (type == "Node2D") {
        auto *child = new Node2D(name);
        *(Node2D**)lua_newuserdata(L, sizeof(Node2D*)) = child;
        luaL_getmetatable(L, "Node2DMetaTable");
        lua_setmetatable(L, -2);
        node->children.push_back(*child);
    }
    else if (type == "Sprite2D") {
        const char *texture = luaL_checkstring(L, 4);
        auto *child = new Sprite2D(name, texture);
        *(Sprite2D**)lua_newuserdata(L, sizeof(Sprite2D*)) = child;
        luaL_getmetatable(L, "Sprite2DMetaTable");
        lua_setmetatable(L, -2);
        node->children.push_back(*child);
    }
    else {
        luaL_error(L, "Invalid type provided.");
    }
    return 1;
}

LUA_API int luau_Node2DGetPosition(lua_State *L)
{
    Node2D* node = *(Node2D**)luaL_checkudata(L, 1, "Node2DMetaTable");
    lua_pushnumber(L, node->position.x);
    lua_pushnumber(L, node->position.y);
    return 2;
}

LUA_API int luau_Node2DSetPosition(lua_State *L)
{
    Node2D* node = *(Node2D**)luaL_checkudata(L, 1, "Node2DMetaTable");
    node->position.x = static_cast<float>(luaL_checknumber(L, 2));
    node->position.y = static_cast<float>(luaL_checknumber(L, 3));
    return 0;
}

LUA_API int luau_Sprite2DSetTexture(lua_State *L)
{
    Sprite2D* sprite = *(Sprite2D**)luaL_checkudata(L, 1, "Sprite2DMetaTable");
    const char *texture = luaL_checkstring(L, 2);
    sprite->SetTexture(texture);
    return 0;
}

/* NODE LIBRARY */

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
    }, name);
    lua_settable(L, -3);

    lua_pushstring(L, "__metatable");
    lua_pushstring(L, "protected");
    lua_settable(L, -3);

    lua_setmetatable(L, -2);

    lua_setglobal(L, name);
}

void luau_ExposeGameInfoTable(lua_State *L, const GameInfo *info)
{
    lua_newtable(L);

    // Populate the "game" table

    // game.name
    lua_pushstring(L, "name");
    lua_pushstring(L, info->getName().c_str());
    lua_settable(L, -3);

    // game.version
    lua_pushstring(L, "version");
    lua_pushstring(L, info->getVersion().c_str());
    lua_settable(L, -3);

    // game.authors
    lua_pushstring(L, "authors");
    lua_createtable(L, 0, info->getAuthors().size());
    for (size_t i = 0; i < info->getAuthors().size(); i++) {
        lua_pushstring(L, info->getAuthors()[i].c_str());
        lua_setfield(L, -2, std::to_string(i + 1).c_str());
    }
    lua_settable(L, -3);

    // game.description
    lua_pushstring(L, "description");
    lua_pushstring(L, info->getDescription().c_str());
    lua_settable(L, -3);

    // game.max_players
    lua_pushstring(L, "max_players");
    lua_pushnumber(L, info->getMaxPlayers());
    lua_settable(L, -3);

    lua_newtable(L);

    lua_pushstring(L, "__index");
    lua_pushvalue(L, -3);
    lua_settable(L, -3);

    lua_pushstring(L, "__newindex");
    lua_pushcfunction(L, [](lua_State* Lm) -> int {
        luaL_error(Lm, "attempt to modify a read-only table");
        return 0;
    }, "game");
    lua_settable(L, -3);

    lua_setmetatable(L, -2);

    lua_setglobal(L, "game");
}

void luau_ExposeConstants(lua_State *L, const Types::VMState state)
{
    lua_pushinteger(L, state == Types::VMState::CLIENT);
    lua_setglobal(L, "CLIENT");

    lua_pushinteger(L, state == Types::VMState::SERVER);
    lua_setglobal(L, "SERVER");

    #ifdef RTYPE_DEBUG
        lua_pushinteger(L, RTYPE_DEBUG);
        lua_setglobal(L, "DEBUG");
    #endif
}

void luau_RegisterNodeMetatables(lua_State* L)
{
    // Add Node metatable
    luaL_newmetatable(L, "NodeMetaTable");
    lua_pushcfunction(L, lua_gcNode, "NodeMetaTable");
    lua_setfield(L, -2, "__gc");
    lua_pop(L, 1);

    // Add Node2D metatable
    luaL_newmetatable(L, "Node2DMetaTable");
    lua_pushcfunction(L, lua_gcNode2D, "Node2DMetaTable");
    lua_setfield(L, -2, "__gc");
    lua_pop(L, 1);

    // Add Sprite2D metatable
    luaL_newmetatable(L, "Sprite2DMetaTable");
    lua_pushcfunction(L, lua_gcSprite2D, "Sprite2DMetaTable");
    lua_setfield(L, -2, "__gc");
    lua_pop(L, 1);
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

    /* NODE LIBRARY */
    luau_RegisterNodeMetatables(L);
    constexpr luaL_Reg nodeLibrary[] = {
        {"GetName", luau_NodeGetName},
        {"SetName", luau_NodeSetName},
        {"GetChildren", luau_NodeGetChildren},
        {"GetChild", luau_NodeGetChild},
        {"AddChild", luau_NodeAddChild},
        {nullptr, nullptr}
    };
    luau_ExposeFunctionsAsMetatable(L, nodeLibrary, "NodeMetaTable", nullptr);
    constexpr luaL_Reg node2DLibrary[] = {
        {"GetPosition", luau_Node2DGetPosition},
        {"SetPosition", luau_Node2DSetPosition},
        {nullptr, nullptr}
    };
    luau_ExposeFunctionsAsMetatable(L, node2DLibrary, "Node2DMetaTable", "NodeMetaTable");
    constexpr luaL_Reg sprite2DLibrary[] = {
        {"SetTexture", luau_Sprite2DSetTexture},
        {nullptr, nullptr}
    };
    luau_ExposeFunctionsAsMetatable(L, sprite2DLibrary, "Sprite2DMetaTable", "Node2DMetaTable");
    /* NODE LIBRARY */
}
