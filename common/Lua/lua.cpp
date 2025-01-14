//
// Created by Théo Bary on 29/11/2024.
//

#include "lua.hpp"
#include "Nodes/UI/Parallax/Parallax.hpp"
#include "Rectangle.hpp"
#include "luaconf.h"

#include <Nodes/Node.hpp>
#include <Nodes/Node2D/CollisionNode2D/Area2D/Area2D.hpp>
#include <Nodes/Node2D/CollisionNode2D/Body2D/RigidBody2D/RigidBody2D.hpp>
#include <Nodes/Node2D/CollisionNode2D/Body2D/StaticBody2D/StaticBody2D.hpp>
#include <Nodes/Node2D/CollisionShape2D/CollisionShape2D.hpp>
#include <Nodes/Node2D/Node2D.hpp>
#include <Nodes/Node2D/Sprite2D/Sprite2D.hpp>
#include <Nodes/Shape2D/Rectangle2D/Rectangle2D.hpp>

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
        char *bytecode = luau_compile(script.c_str(), script.size(), nullptr, &bytecodeSize);

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

        lua_remove(L, 1);

        spdlog::debug("Successfully loaded file: {}", filePath);
        return lua_gettop(L);
    }
    catch (const std::exception &e) {
        lua_pushfstring(L, "Exception: %s", e.what());
        lua_error(L);
    }
}

LUA_API int luau_Info(lua_State *L)
{
    const char *msg = lua_tostring(L, 1);
    if (!msg) {
        lua_pushstring(L, "Invalid info message provided.");
        lua_error(L);
    }
    spdlog::info(msg);
    return lua_gettop(L);
}

LUA_API int luau_Error(lua_State *L)
{
    const char *msg = lua_tostring(L, 1);
    if (!msg) {
        lua_pushstring(L, "Invalid error message provided.");
        lua_error(L);
    }
    spdlog::error(msg);
    return lua_gettop(L);
}

LUA_API int luau_Warn(lua_State *L)
{
    const char *msg = lua_tostring(L, 1);
    if (!msg) {
        lua_pushstring(L, "Invalid warn message provided.");
        lua_error(L);
    }
    spdlog::warn(msg);
    return lua_gettop(L);
}

LUA_API int luau_Debug(lua_State *L)
{
    const char *msg = lua_tostring(L, 1);
    if (!msg) {
        lua_pushstring(L, "Invalid debug message provided.");
        lua_error(L);
    }
    spdlog::debug(msg);
    return lua_gettop(L);
}

/* NET LIBRARY */
LUA_API int luau_NetCreatePacket(lua_State *L)
{
    const char *packetName = lua_tostring(L, 1);
    if (!packetName) {
        lua_pushstring(L, "Invalid packet name provided.");
        lua_error(L);
    }
    if (Engine::GetInstance().hasPacketRegistryEntry(packetName)) {
        lua_pushstring(L, "Packet already exists.");
        lua_error(L);
    }
    const bool reliable = lua_toboolean(L, 2);
    Engine::GetInstance().addPacketRegistryEntry(packetName, reliable);
    return lua_gettop(L);
}

LUA_API int luau_NetStart(lua_State *L)
{
    const char *packetName = lua_tostring(L, 1);
    if (!packetName) {
        lua_pushstring(L, "Invalid packet name provided.");
        lua_error(L);
    }
    if (!Engine::GetInstance().hasPacketRegistryEntry(packetName)) {
        lua_pushstring(
            L,
            std::format("Packet \"{}\" has not been initialized. Try calling net.CreatePacket first.", packetName)
                .c_str());
        lua_error(L);
    }
    PacketBuilder &builder = Engine::GetInstance().getPacketBuilders().emplace();
    builder.setCmd(PacketCmd::NET).writeString(packetName);
    Engine::GetInstance().getLastStartedPacket() = packetName;
    return lua_gettop(L);
}

LUA_API int luau_NetSendToServer(lua_State *L)
{
    if (Engine::GetInstance().getState() != Types::VMState::CLIENT) {
        lua_pushstring(L, "This function can only be called on the client.");
        lua_error(L);
    }
    Engine::GetInstance().getBroadcastQueue().emplace(Engine::GetInstance().getLastStartedPacket(),
                                                      Engine::GetInstance().getPacketBuilders().top().build());
    Engine::GetInstance().getPacketBuilders().pop();
    return lua_gettop(L);
}

LUA_API int luau_NetSendToClient(lua_State *L)
{
    const char *clientUUID = lua_tostring(L, 1);

    if (Engine::GetInstance().getState() != Types::VMState::SERVER) {
        lua_pushstring(L, "This function can only be called on the server.");
        lua_error(L);
    }
    if (!clientUUID) {
        lua_pushstring(L, "Invalid client UUID provided.");
        lua_error(L);
    }
    Engine::GetInstance().getSendToClientMap()[clientUUID].emplace(
        Engine::GetInstance().getLastStartedPacket(), Engine::GetInstance().getPacketBuilders().top().build());
    Engine::GetInstance().getPacketBuilders().pop();
    return lua_gettop(L);
}

LUA_API int luau_NetBroadcast(lua_State *L)
{
    if (Engine::GetInstance().getState() != Types::VMState::SERVER) {
        lua_pushstring(L, "This function can only be called on the server.");
        lua_error(L);
    }

    Engine::GetInstance().getBroadcastQueue().emplace(Engine::GetInstance().getLastStartedPacket(),
                                                      Engine::GetInstance().getPacketBuilders().top().build());
    Engine::GetInstance().getPacketBuilders().pop();
    return lua_gettop(L);
}

/* READ/WRITE OPERATIONS */ // ALL OPERATIONS HERE MUST HAVE A READ AND WRITE PAIR
LUA_API int luau_NetWriteInt(lua_State *L)
{
    const int nb = lua_tointeger(L, 1);
    PacketBuilder &builder = Engine::GetInstance().getPacketBuilders().top();
    builder.writeInt(nb);
    return lua_gettop(L);
}

LUA_API int luau_NetReadInt(lua_State *L)
{
    PacketBuilder &builder = Engine::GetInstance().getPacketBuilders().top();
    const int nb = builder.readInt();
    lua_pushinteger(L, nb);
    return lua_gettop(L);
}

LUA_API int luau_NetWriteFloat(lua_State *L)
{
    const float nb = lua_tonumber(L, 1);
    PacketBuilder &builder = Engine::GetInstance().getPacketBuilders().top();
    builder.writeFloat(nb);
    return lua_gettop(L);
}

LUA_API int luau_NetReadFloat(lua_State *L)
{
    PacketBuilder &builder = Engine::GetInstance().getPacketBuilders().top();
    const float nb = builder.readFloat();
    lua_pushnumber(L, nb);
    return lua_gettop(L);
}

LUA_API int luau_NetWriteString(lua_State *L)
{
    const char *str = lua_tostring(L, 1);
    if (!str) {
        lua_pushstring(L, "Invalid string provided.");
        lua_error(L);
    }
    PacketBuilder &builder = Engine::GetInstance().getPacketBuilders().top();
    builder.writeString(str);
    return lua_gettop(L);
}

LUA_API int luau_NetReadString(lua_State *L)
{
    PacketBuilder &builder = Engine::GetInstance().getPacketBuilders().top();
    const std::string str = builder.readString();
    lua_pushstring(L, str.c_str());
    return lua_gettop(L);
}
/* READ/WRITE OPERATIONS */

/* NET LIBRARY */

/* NODE LIBRARY */
void luau_ExposeRootNode(lua_State *L)
{
    Node *root = Engine::GetInstance().root;
    if (root == nullptr)
        throw std::runtime_error("Root node not initialized.");
    *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = root;
    luaL_getmetatable(L, "NodeMetaTable");
    lua_setmetatable(L, -2);
    lua_setglobal(L, "root");
}

/** __gc functions **/

LUA_API int lua_gcNode(lua_State *L)
{
    Node *node = *static_cast<Node **>(luaL_checkudata(L, 1, "NodeMetaTable"));
    delete node;
    return 0;
}

LUA_API int lua_gcNode2D(lua_State *L)
{
    Node2D *node2D = *static_cast<Node2D **>(luaL_checkudata(L, 1, "Node2DMetaTable"));
    delete node2D;
    return 0;
}

LUA_API int lua_gcCollisionShape2D(lua_State *L)
{
    CollisionShape2D *node2D = *static_cast<CollisionShape2D **>(luaL_checkudata(L, 1, "CollisionShape2DMetaTable"));
    delete node2D;
    return 0;
}

LUA_API int lua_gcSprite2D(lua_State *L)
{
    Sprite2D *sprite2D = *static_cast<Sprite2D **>(luaL_checkudata(L, 1, "Sprite2DMetaTable"));
    delete sprite2D;
    return 0;
}

LUA_API int lua_gcArea2D(lua_State *L)
{
    Area2D *area2D = *static_cast<Area2D **>(luaL_checkudata(L, 1, "Area2DMetaTable"));
    delete area2D;
    return 0;
}

LUA_API int lua_gcParallax(lua_State *L)
{
    Parallax *parallax = *static_cast<Parallax **>(luaL_checkudata(L, 1, "ParallaxMetaTable"));
    delete parallax;
    return 0;
}

LUA_API int lua_gcRigidBody2D(lua_State *L)
{
    RigidBody2D *rigidBody2D = *static_cast<RigidBody2D **>(luaL_checkudata(L, 1, "RigidBody2DMetaTable"));
    delete rigidBody2D;
    return 0;
}

LUA_API int lua_gcStaticBody2D(lua_State *L)
{
    StaticBody2D *staticBody2D = *static_cast<StaticBody2D **>(luaL_checkudata(L, 1, "StaticBody2DMetaTable"));
    delete staticBody2D;
    return 0;
}

/** __gc functions **/

/** GetName functions **/

template <typename T>
LUA_API int luau_TemplateNodeGetName(lua_State *L, const char *metaTableName)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metaTableName));
    lua_pushstring(L, node->name.c_str());
    return 1;
}

LUA_API int luau_NodeGetName(lua_State *L) { return luau_TemplateNodeGetName<Node>(L, "NodeMetaTable"); }

LUA_API int luau_Node2DGetName(lua_State *L) { return luau_TemplateNodeGetName<Node2D>(L, "Node2DMetaTable"); }

LUA_API int luau_CollisionShape2DGetName(lua_State *L)
{
    return luau_TemplateNodeGetName<CollisionShape2D>(L, "CollisionShape2DMetaTable");
}

LUA_API int luau_Sprite2DGetName(lua_State *L) { return luau_TemplateNodeGetName<Sprite2D>(L, "Sprite2DMetaTable"); }

LUA_API int luau_Area2DGetName(lua_State *L) { return luau_TemplateNodeGetName<Area2D>(L, "Area2DMetaTable"); }

LUA_API int luau_ParallaxGetName(lua_State *L) { return luau_TemplateNodeGetName<Parallax>(L, "ParallaxMetaTable"); }

LUA_API int luau_RigidBody2DGetName(lua_State *L)
{
    return luau_TemplateNodeGetName<RigidBody2D>(L, "RigidBody2DMetaTable");
}

LUA_API int luau_StaticBody2DGetName(lua_State *L)
{
    return luau_TemplateNodeGetName<StaticBody2D>(L, "StaticBody2DMetaTable");
}

/** GetName functions **/

/** SetName functions **/

LUA_API int luau_NodeSetName(lua_State *L)
{
    Node *node = *static_cast<Node **>(luaL_checkudata(L, 1, "NodeMetaTable"));
    const char *name = luaL_checkstring(L, 2);
    node->name = name;
    return 0;
}

LUA_API int luau_Node2DSetName(lua_State *L)
{
    Node2D *node = *static_cast<Node2D **>(luaL_checkudata(L, 1, "Node2DMetaTable"));
    const char *name = luaL_checkstring(L, 2);
    node->name = name;
    return 0;
}

LUA_API int luau_CollisionShape2DSetName(lua_State *L)
{
    CollisionShape2D *node = *static_cast<CollisionShape2D **>(luaL_checkudata(L, 1, "CollisionShape2DMetaTable"));
    const char *name = luaL_checkstring(L, 2);
    node->name = name;
    return 0;
}

LUA_API int luau_Sprite2DSetName(lua_State *L)
{
    Sprite2D *sprite = *static_cast<Sprite2D **>(luaL_checkudata(L, 1, "Sprite2DMetaTable"));
    const char *name = luaL_checkstring(L, 2);
    sprite->name = name;
    return 0;
}

LUA_API int luau_Area2DSetName(lua_State *L)
{
    Area2D *area2D = *static_cast<Area2D **>(luaL_checkudata(L, 1, "Area2DMetaTable"));
    const char *name = luaL_checkstring(L, 2);
    area2D->name = name;
    return 0;
}

LUA_API int luau_ParallaxSetName(lua_State *L)
{
    Parallax *parallax = *static_cast<Parallax **>(luaL_checkudata(L, 1, "ParallaxMetaTable"));
    const char *name = luaL_checkstring(L, 2);
    parallax->name = name;
    return 0;
}
LUA_API int luau_RigidBody2DSetName(lua_State *L)
{
    RigidBody2D *rgbd2D = *static_cast<RigidBody2D **>(luaL_checkudata(L, 1, "RigidBody2DMetaTable"));
    const char *name = luaL_checkstring(L, 2);
    rgbd2D->name = name;
    return 0;
}

LUA_API int luau_StaticBody2DSetName(lua_State *L)
{
    StaticBody2D *staticBody2D = *static_cast<StaticBody2D **>(luaL_checkudata(L, 1, "StaticBody2DMetaTable"));
    const char *name = luaL_checkstring(L, 2);
    staticBody2D->name = name;
    return 0;
}

/** SetName functions **/

/** GetChildren functions **/

template <typename T>
LUA_API int luau_TemplateNodeGetChildren(lua_State *L, const char *metaTableName)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metaTableName));
    std::vector<Node *> children = node->children;
    lua_newtable(L);
    for (size_t i = 0; i < children.size(); i++) {
        *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = children[i];
        luaL_getmetatable(L, children[i]->metatable.c_str());
        lua_setmetatable(L, -2);
        lua_rawseti(L, -2, i + 1);
    }
    return 1;
}

LUA_API int luau_NodeGetChildren(lua_State *L) { return luau_TemplateNodeGetChildren<Node>(L, "NodeMetaTable"); }

LUA_API int luau_Node2DGetChildren(lua_State *L) { return luau_TemplateNodeGetChildren<Node2D>(L, "Node2DMetaTable"); }

LUA_API int luau_CollisionShape2DGetChildren(lua_State *L)
{
    return luau_TemplateNodeGetChildren<CollisionShape2D>(L, "CollisionShape2DMetaTable");
}

LUA_API int luau_Sprite2DGetChildren(lua_State *L)
{
    return luau_TemplateNodeGetChildren<Sprite2D>(L, "Sprite2DMetaTable");
}

LUA_API int luau_Area2DGetChildren(lua_State *L) { return luau_TemplateNodeGetChildren<Area2D>(L, "Area2DMetaTable"); }

LUA_API int luau_ParallaxGetChildren(lua_State *L)
{
    return luau_TemplateNodeGetChildren<Parallax>(L, "ParallaxMetaTable");
}

LUA_API int luau_RigidBody2DGetChildren(lua_State *L)
{
    return luau_TemplateNodeGetChildren<RigidBody2D>(L, "RigidBody2DMetaTable");
}

LUA_API int luau_StaticBody2DGetChildren(lua_State *L)
{
    return luau_TemplateNodeGetChildren<StaticBody2D>(L, "StaticBody2DMetaTable");
}

/** GetChildren functions **/

/** GetChild functions **/

static Node *luau_NodeFactory(lua_State *L, const std::string &type)
{
    const char *name = luaL_checkstring(L, 3);
    Node *child = nullptr;

    if (type == "Node") {
        child = new Node(name);
    }
    else if (type == "Node2D") {
        child = new Node2D(name);
    }
    else if (type == "Sprite2D") {
        const char *texture = luaL_checkstring(L, 4);
        child = new Sprite2D(name, texture);
    }
    else if (type == "CollisionShape2D") {
        const char *shapeType = luaL_checkstring(L, 4);
        Shape2D *shape;
        if (std::strcmp(shapeType, "Rectangle") == 0) {
            double x = luaL_checknumber(L, 5);
            double y = luaL_checknumber(L, 6);
            double width = luaL_checknumber(L, 7);
            double height = luaL_checknumber(L, 8);
            shape = new Rectangle2D(Types::Vector2(x, y), Types::Vector2(width, height));
        }
        else {
            luaL_error(L, "Invalid shape type '%s' provided to AddChild in CollisionShape2D.", shapeType);
        }
        child = new CollisionShape2D(name);
        dynamic_cast<CollisionShape2D *>(child)->setShape(shape);
    }
    else if (type == "Area2D") {
        double x = luaL_checknumber(L, 4);
        double y = luaL_checknumber(L, 5);
        double width = luaL_checknumber(L, 6);
        double height = luaL_checknumber(L, 7);
        child = new Area2D(name, Types::Vector2(x, y), Types::Vector2(width, height));
    }
    else if (type == "Parallax") {
        const char *texture = luaL_checkstring(L, 4);
        int zIndex = luaL_checkinteger(L, 5);
        Node2D *referenceNode = nullptr;
        if (!lua_isnil(L, 6)) {
            Node2D *referenceNode = *static_cast<Node2D **>(luaL_checkudata(L, 6, "Node2DMetaTable"));
        }
        child = new Parallax(texture, name, zIndex, referenceNode);
    }
    else if (type == "StaticBody2D") {
        double x = luaL_checknumber(L, 4);
        double y = luaL_checknumber(L, 5);
        child = new StaticBody2D(name, Types::Vector2(x, y));
    }
    else if (type == "RigidBody2D") {
        double x = luaL_checknumber(L, 4);
        double y = luaL_checknumber(L, 5);
        double vel_x = luaL_checknumber(L, 6);
        double vel_y = luaL_checknumber(L, 7);
        child = new RigidBody2D(name, Types::Vector2(x, y), Types::Vector2(vel_x, vel_y));
    }
    else {
        luaL_error(L, "Invalid type '%s' provided to AddChild in Node.", type.c_str());
    }
    return child;
}

LUA_API int luau_NodeGetChild(lua_State *L)
{
    Node *node = *static_cast<Node **>(luaL_checkudata(L, 1, "NodeMetaTable"));
    const char *childName = luaL_checkstring(L, 2);
    int find = 0;

    std::vector<Node *> &children = node->children;
    for (auto &child : children) {
        if (child->name == childName) {
            *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = child;
            luaL_getmetatable(L, child->metatable.c_str());
            lua_setmetatable(L, -2);
            find = 1;
        }
    }
    if (!find)
        lua_pushnil(L);
    return 1;
}

LUA_API int luau_Node2DGetChild(lua_State *L)
{
    Node2D *node = *static_cast<Node2D **>(luaL_checkudata(L, 1, "Node2DMetaTable"));
    const char *childName = luaL_checkstring(L, 2);
    int find = 0;

    std::vector<Node *> &children = node->children;
    for (auto &child : children) {
        if (child->name == childName) {
            *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = child;
            luaL_getmetatable(L, child->metatable.c_str());
            lua_setmetatable(L, -2);
            find = 1;
        }
    }
    if (!find)
        lua_pushnil(L);
    return 1;
}

LUA_API int luau_CollisionShape2DGetChild(lua_State *L)
{
    CollisionShape2D *node = *static_cast<CollisionShape2D **>(luaL_checkudata(L, 1, "CollisionShape2DMetaTable"));
    const char *childName = luaL_checkstring(L, 2);
    int find = 0;

    std::vector<Node *> &children = node->children;
    for (auto &child : children) {
        if (child->name == childName) {
            *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = child;
            luaL_getmetatable(L, child->metatable.c_str());
            lua_setmetatable(L, -2);
            find = 1;
        }
    }
    if (!find)
        lua_pushnil(L);
    return 1;
}

LUA_API int luau_Sprite2DGetChild(lua_State *L)
{
    Sprite2D *node = *static_cast<Sprite2D **>(luaL_checkudata(L, 1, "Sprite2DMetaTable"));
    const char *childName = luaL_checkstring(L, 2);
    int find = 0;

    std::vector<Node *> &children = node->children;
    for (auto &child : children) {
        if (child->name == childName) {
            *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = child;
            luaL_getmetatable(L, child->metatable.c_str());
            lua_setmetatable(L, -2);
            find = 1;
        }
    }
    if (!find)
        lua_pushnil(L);
    return 1;
}

LUA_API int luau_Area2DGetChild(lua_State *L)
{
    Area2D *node = *static_cast<Area2D **>(luaL_checkudata(L, 1, "Area2DMetaTable"));
    const char *childName = luaL_checkstring(L, 2);
    int find = 0;

    std::vector<Node *> &children = node->children;
    for (auto &child : children) {
        if (child->name == childName) {
            *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = child;
            luaL_getmetatable(L, child->metatable.c_str());
            lua_setmetatable(L, -2);
            find = 1;
        }
    }
    if (!find)
        lua_pushnil(L);
    return 1;
}

LUA_API int luau_ParallaxGetChild(lua_State *L)
{
    Parallax *node = *static_cast<Parallax **>(luaL_checkudata(L, 1, "ParallaxMetaTable"));
    const char *childName = luaL_checkstring(L, 2);
    int find = 0;

    std::vector<Node *> &children = node->children;
    for (auto &child : children) {
        if (child->name == childName) {
            *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = child;
            luaL_getmetatable(L, child->metatable.c_str());
            lua_setmetatable(L, -2);
            find = 1;
        }
    }
    if (!find)
        lua_pushnil(L);
    return 1;
}

LUA_API int luau_RigidBody2DGetChild(lua_State *L)
{
    RigidBody2D *node = *static_cast<RigidBody2D **>(luaL_checkudata(L, 1, "RigidBody2DMetaTable"));
    const char *childName = luaL_checkstring(L, 2);
    int find = 0;

    std::vector<Node *> &children = node->children;
    for (auto &child : children) {
        if (child->name == childName) {
            *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = child;
            luaL_getmetatable(L, child->metatable.c_str());
            lua_setmetatable(L, -2);
            find = 1;
        }
    }
    if (!find)
        lua_pushnil(L);
    return 1;
}

LUA_API int luau_StaticBody2DGetChild(lua_State *L)
{
    StaticBody2D *node = *static_cast<StaticBody2D **>(luaL_checkudata(L, 1, "StaticBody2DMetaTable"));
    const char *childName = luaL_checkstring(L, 2);
    int find = 0;

    std::vector<Node *> &children = node->children;
    for (auto &child : children) {
        if (child->name == childName) {
            *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = child;
            luaL_getmetatable(L, child->metatable.c_str());
            lua_setmetatable(L, -2);
            find = 1;
        }
    }
    if (!find)
        lua_pushnil(L);
    return 1;
}

/** GetChild functions **/

/** CreateChild functions **/

LUA_API int luau_NodeCreateChild(lua_State *L)
{
    Node *node = *static_cast<Node **>(luaL_checkudata(L, 1, "NodeMetaTable"));
    const std::string type = luaL_checkstring(L, 2);

    Node *child = luau_NodeFactory(L, type);
    *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = child;
    std::string metatableName = type + "MetaTable";
    luaL_getmetatable(L, metatableName.c_str());
    if (lua_isnil(L, -1)) {
        luaL_error(L, "Metatable '%s' not found. Ensure it is registered before calling AddChild.",
                   metatableName.c_str());
    }
    lua_setmetatable(L, -2);
    node->addChild(*child);
    return 1;
}

LUA_API int luau_Node2DCreateChild(lua_State *L)
{
    Node2D *node = *static_cast<Node2D **>(luaL_checkudata(L, 1, "Node2DMetaTable"));
    const std::string type = luaL_checkstring(L, 2);

    Node *child = luau_NodeFactory(L, type);
    *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = child;
    std::string metatableName = type + "MetaTable";
    luaL_getmetatable(L, metatableName.c_str());
    if (lua_isnil(L, -1)) {
        luaL_error(L, "Metatable '%s' not found. Ensure it is registered before calling AddChild.",
                   metatableName.c_str());
    }
    lua_setmetatable(L, -2);
    node->addChild(*child);
    return 1;
}

LUA_API int luau_CollisionShape2DCreateChild(lua_State *L)
{
    CollisionShape2D *node = *static_cast<CollisionShape2D **>(luaL_checkudata(L, 1, "CollisionShape2DMetaTable"));
    const std::string type = luaL_checkstring(L, 2);

    Node *child = luau_NodeFactory(L, type);
    *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = child;
    std::string metatableName = type + "MetaTable";
    luaL_getmetatable(L, metatableName.c_str());
    if (lua_isnil(L, -1)) {
        luaL_error(L, "Metatable '%s' not found. Ensure it is registered before calling AddChild.",
                   metatableName.c_str());
    }
    lua_setmetatable(L, -2);
    node->addChild(*child);
    return 1;
}

LUA_API int luau_Sprite2DCreateChild(lua_State *L)
{
    Sprite2D *node = *static_cast<Sprite2D **>(luaL_checkudata(L, 1, "Sprite2DMetaTable"));
    const std::string type = luaL_checkstring(L, 2);

    Node *child = luau_NodeFactory(L, type);
    *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = child;
    std::string metatableName = type + "MetaTable";
    luaL_getmetatable(L, metatableName.c_str());
    if (lua_isnil(L, -1)) {
        luaL_error(L, "Metatable '%s' not found. Ensure it is registered before calling AddChild.",
                   metatableName.c_str());
    }
    lua_setmetatable(L, -2);
    node->addChild(*child);
    return 1;
}

LUA_API int luau_Area2DCreateChild(lua_State *L)
{
    Area2D *node = *static_cast<Area2D **>(luaL_checkudata(L, 1, "Area2DMetaTable"));
    const std::string type = luaL_checkstring(L, 2);

    Node *child = luau_NodeFactory(L, type);
    *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = child;
    std::string metatableName = type + "MetaTable";
    luaL_getmetatable(L, metatableName.c_str());
    if (lua_isnil(L, -1)) {
        luaL_error(L, "Metatable '%s' not found. Ensure it is registered before calling AddChild.",
                   metatableName.c_str());
    }
    lua_setmetatable(L, -2);
    node->addChild(*child);
    return 1;
}

LUA_API int luau_ParallaxCreateChild(lua_State *L)
{
    Parallax *node = *static_cast<Parallax **>(luaL_checkudata(L, 1, "ParallaxMetaTable"));
    const std::string type = luaL_checkstring(L, 2);

    Node *child = luau_NodeFactory(L, type);
    *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = child;
    std::string metatableName = type + "MetaTable";
    luaL_getmetatable(L, metatableName.c_str());
    if (lua_isnil(L, -1)) {
        luaL_error(L, "Metatable '%s' not found. Ensure it is registered before calling AddChild.",
                   metatableName.c_str());
    }
    lua_setmetatable(L, -2);
    node->addChild(*child);
    return 1;
}

LUA_API int luau_RigidBody2DCreateChild(lua_State *L)
{
    RigidBody2D *node = *static_cast<RigidBody2D **>(luaL_checkudata(L, 1, "RigidBody2DMetaTable"));
    const std::string type = luaL_checkstring(L, 2);

    Node *child = luau_NodeFactory(L, type);
    *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = child;
    std::string metatableName = type + "MetaTable";
    luaL_getmetatable(L, metatableName.c_str());
    if (lua_isnil(L, -1)) {
        luaL_error(L, "Metatable '%s' not found. Ensure it is registered before calling AddChild.",
                   metatableName.c_str());
    }
    lua_setmetatable(L, -2);
    node->addChild(*child);
    return 1;
}

LUA_API int luau_StaticBody2DCreateChild(lua_State *L)
{
    StaticBody2D *node = *static_cast<StaticBody2D **>(luaL_checkudata(L, 1, "StaticBody2DMetaTable"));
    const std::string type = luaL_checkstring(L, 2);

    Node *child = luau_NodeFactory(L, type);
    *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = child;
    std::string metatableName = type + "MetaTable";
    luaL_getmetatable(L, metatableName.c_str());
    if (lua_isnil(L, -1)) {
        luaL_error(L, "Metatable '%s' not found. Ensure it is registered before calling AddChild.",
                   metatableName.c_str());
    }
    lua_setmetatable(L, -2);
    node->addChild(*child);
    return 1;
}

/** CreateChild functions **/

/** AddChild functions **/

LUA_API int luau_NodeAddChild(lua_State *L)
{
    Node *node = *static_cast<Node **>(luaL_checkudata(L, 1, "NodeMetaTable"));
    Node *child = *static_cast<Node **>(luaL_checkudata(L, 2, "NodeMetaTable"));
    node->addChild(*child);
    return 0;
}

LUA_API int luau_Node2DAddChild(lua_State *L)
{
    Node2D *node = *static_cast<Node2D **>(luaL_checkudata(L, 1, "Node2DMetaTable"));
    Node *child = *static_cast<Node **>(luaL_checkudata(L, 2, "NodeMetaTable"));
    node->addChild(*child);
    return 0;
}

LUA_API int luau_CollisionShape2DAddChild(lua_State *L)
{
    CollisionShape2D *node = *static_cast<CollisionShape2D **>(luaL_checkudata(L, 1, "CollisionShape2DMetaTable"));
    Node *child = *static_cast<Node **>(luaL_checkudata(L, 2, "NodeMetaTable"));
    node->addChild(*child);
    return 0;
}

LUA_API int luau_Sprite2DAddChild(lua_State *L)
{
    Sprite2D *node = *static_cast<Sprite2D **>(luaL_checkudata(L, 1, "Node2DMetaTable"));
    Node *child = *static_cast<Node **>(luaL_checkudata(L, 2, "NodeMetaTable"));
    node->addChild(*child);
    return 0;
}

LUA_API int luau_Area2DAddChild(lua_State *L)
{
    Area2D *node = *static_cast<Area2D **>(luaL_checkudata(L, 1, "Area2DMetaTable"));
    Node *child = *static_cast<Node **>(luaL_checkudata(L, 2, "NodeMetaTable"));
    node->addChild(*child);
    return 0;
}

LUA_API int luau_ParallaxAddChild(lua_State *L)
{
    Parallax *node = *static_cast<Parallax **>(luaL_checkudata(L, 1, "ParallaxMetaTable"));
    Node *child = *static_cast<Node **>(luaL_checkudata(L, 2, "NodeMetaTable"));
    node->addChild(*child);
    return 0;
}

LUA_API int luau_RigidBody2DAddChild(lua_State *L)
{
    RigidBody2D *node = *static_cast<RigidBody2D **>(luaL_checkudata(L, 1, "RigidBody2DMetaTable"));
    Node *child = *static_cast<Node **>(luaL_checkudata(L, 2, "NodeMetaTable"));
    node->addChild(*child);
    return 0;
}

LUA_API int luau_StaticBody2DAddChild(lua_State *L)
{
    StaticBody2D *node = *static_cast<StaticBody2D **>(luaL_checkudata(L, 1, "StaticBody2DMetaTable"));
    Node *child = *static_cast<Node **>(luaL_checkudata(L, 2, "NodeMetaTable"));
    node->addChild(*child);
    return 0;
}

/** AddChild functions **/

/** Update functions **/

template <typename T>
LUA_API int luau_TemplatedNodeUpdate(lua_State *L, const char *metaTableName)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metaTableName));
    node->Update();
    return 0;
}

LUA_API int luau_Node2DUpdate(lua_State *L) { return luau_TemplatedNodeUpdate<Node2D>(L, "Node2DMetaTable"); }

LUA_API int luau_CollisionShape2DUpdate(lua_State *L)
{
    return luau_TemplatedNodeUpdate<CollisionShape2D>(L, "CollisionShape2DMetaTable");
}

LUA_API int luau_Sprite2DUpdate(lua_State *L) { return luau_TemplatedNodeUpdate<Sprite2D>(L, "Sprite2DMetaTable"); }

LUA_API int luau_Area2DUpdate(lua_State *L) { return luau_TemplatedNodeUpdate<Area2D>(L, "Area2DMetaTable"); }

LUA_API int luau_ParallaxUpdate(lua_State *L) { return luau_TemplatedNodeUpdate<Parallax>(L, "ParallaxMetaTable"); }

LUA_API int luau_RigidBody2DUpdate(lua_State *L)
{
    return luau_TemplatedNodeUpdate<RigidBody2D>(L, "RigidBody2DMetaTable");
}

LUA_API int luau_NodeUpdate(lua_State *L) { return luau_TemplatedNodeUpdate<Node>(L, "NodeMetaTable"); }

LUA_API int luau_StaticBody2DUpdate(lua_State *L)
{
    return luau_TemplatedNodeUpdate<StaticBody2D>(L, "StaticBody2DMetaTable");
}

/** Update functions **/

/** GetPosition functions **/

LUA_API int luau_Node2DGetPosition(lua_State *L)
{
    Node2D *node = *static_cast<Node2D **>(luaL_checkudata(L, 1, "Node2DMetaTable"));
    lua_pushnumber(L, node->position.x);
    lua_pushnumber(L, node->position.y);
    return 2;
}

LUA_API int luau_CollisionShape2DGetPosition(lua_State *L)
{
    CollisionShape2D *node = *static_cast<CollisionShape2D **>(luaL_checkudata(L, 1, "CollisionShape2DMetaTable"));
    lua_pushnumber(L, node->position.x);
    lua_pushnumber(L, node->position.y);
    return 2;
}

LUA_API int luau_Sprite2DGetPosition(lua_State *L)
{
    Sprite2D *sprite = *static_cast<Sprite2D **>(luaL_checkudata(L, 1, "Sprite2DMetaTable"));
    lua_pushnumber(L, sprite->position.x);
    lua_pushnumber(L, sprite->position.y);
    return 2;
}

LUA_API int luau_Area2DGetPosition(lua_State *L)
{
    Area2D *area2D = *static_cast<Area2D **>(luaL_checkudata(L, 1, "Area2DMetaTable"));
    lua_pushnumber(L, area2D->position.x);
    lua_pushnumber(L, area2D->position.y);
    return 2;
}

LUA_API int luau_RigidBody2DGetPosition(lua_State *L)
{
    RigidBody2D *rgbd2D = *static_cast<RigidBody2D **>(luaL_checkudata(L, 1, "RigidBody2DMetaTable"));
    lua_pushnumber(L, rgbd2D->position.x);
    lua_pushnumber(L, rgbd2D->position.y);
    return 2;
}

LUA_API int luau_StaticBody2DGetPosition(lua_State *L)
{
    StaticBody2D *stbd2D = *static_cast<StaticBody2D **>(luaL_checkudata(L, 1, "StaticBody2DMetaTable"));
    lua_pushnumber(L, stbd2D->position.x);
    lua_pushnumber(L, stbd2D->position.y);
    return 2;
}

/** GetPosition functions **/

/** SetPosition functions **/

LUA_API int luau_Node2DSetPosition(lua_State *L)
{
    Node2D *node = *static_cast<Node2D **>(luaL_checkudata(L, 1, "Node2DMetaTable"));
    node->position.x = static_cast<float>(luaL_checknumber(L, 2));
    node->position.y = static_cast<float>(luaL_checknumber(L, 3));
    return 0;
}

LUA_API int luau_CollisionShape2DSetPosition(lua_State *L)
{
    CollisionShape2D *node = *static_cast<CollisionShape2D **>(luaL_checkudata(L, 1, "CollisionShape2DMetaTable"));
    node->position.x = static_cast<float>(luaL_checknumber(L, 2));
    node->position.y = static_cast<float>(luaL_checknumber(L, 3));
    return 0;
}

LUA_API int luau_Sprite2DSetPosition(lua_State *L)
{
    Sprite2D *sprite = *static_cast<Sprite2D **>(luaL_checkudata(L, 1, "Sprite2DMetaTable"));
    sprite->position.x = static_cast<float>(luaL_checknumber(L, 2));
    sprite->position.y = static_cast<float>(luaL_checknumber(L, 3));
    return 0;
}

LUA_API int luau_Area2DSetPosition(lua_State *L)
{
    Area2D *area2D = *static_cast<Area2D **>(luaL_checkudata(L, 1, "Area2DMetaTable"));
    area2D->position.x = static_cast<float>(luaL_checknumber(L, 2));
    area2D->position.y = static_cast<float>(luaL_checknumber(L, 3));
    return 0;
}

LUA_API int luau_RigidBody2DSetPosition(lua_State *L)
{
    RigidBody2D *rgbd2D = *static_cast<RigidBody2D **>(luaL_checkudata(L, 1, "RigidBody2DMetaTable"));
    rgbd2D->position.x = static_cast<float>(luaL_checknumber(L, 2));
    rgbd2D->position.y = static_cast<float>(luaL_checknumber(L, 3));
    return 0;
}

LUA_API int luau_StaticBody2DSetPosition(lua_State *L)
{
    StaticBody2D *stbd2D = *static_cast<StaticBody2D **>(luaL_checkudata(L, 1, "StaticBody2DMetaTable"));
    stbd2D->position.x = static_cast<float>(luaL_checknumber(L, 2));
    stbd2D->position.y = static_cast<float>(luaL_checknumber(L, 3));
    return 0;
}

/** SetPosition functions **/

/** SetTexture functions **/

LUA_API int luau_Sprite2DSetTexture(lua_State *L)
{
    Sprite2D *sprite = *static_cast<Sprite2D **>(luaL_checkudata(L, 1, "Sprite2DMetaTable"));
    const char *texture = luaL_checkstring(L, 2);
    sprite->SetTexture(texture);
    return 0;
}

LUA_API int luau_Sprite2DSetSource(lua_State *L)
{
    Sprite2D *sprite = *static_cast<Sprite2D **>(luaL_checkudata(L, 1, "Sprite2DMetaTable"));
    double x = luaL_checknumber(L, 2);
    double y = luaL_checknumber(L, 3);
    double width = luaL_checknumber(L, 4);
    double height = luaL_checknumber(L, 5);
    Types::Rect2 source = {Types::Vector2(x, y), Types::Vector2(width, height)};
    sprite->SetSource(source);
    return 0;
}

/** SetTexture functions **/

/** SetTexture functions **/

LUA_API int luau_Sprite2DDraw(lua_State *L)
{
    Sprite2D *sprite = *static_cast<Sprite2D **>(luaL_checkudata(L, 1, "Sprite2DMetaTable"));
    sprite->Draw();
    return 0;
}

/** SetTexture functions **/

/** Collide functions **/

template <typename T>
LUA_API int luau_TemplateCollisionShape2DCollide(lua_State *L, const char *metaTableName)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metaTableName));
    T *other = *static_cast<T **>(luaL_checkudata(L, 2, metaTableName));
    lua_pushboolean(L, node->collidesWith(*other));
    return 1;
}

LUA_API int luau_CollisionShape2DCollide(lua_State *L)
{
    return luau_TemplateCollisionShape2DCollide<CollisionShape2D>(L, "CollisionShape2DMetaTable");
}

LUA_API int luau_Area2DCollide(lua_State *L)
{
    return luau_TemplateCollisionShape2DCollide<Area2D>(L, "Area2DMetaTable");
}

LUA_API int luau_RigidBody2DCollide(lua_State *L)
{
    return luau_TemplateCollisionShape2DCollide<RigidBody2D>(L, "RigidBody2DMetaTable");
}

LUA_API int luau_StaticBody2DCollide(lua_State *L)
{
    return luau_TemplateCollisionShape2DCollide<StaticBody2D>(L, "StaticBody2DMetaTable");
}

/** Collide functions **/

/** ToggleCollision functions **/

template <typename T>
LUA_API int luau_TemplateCollisionShape2DToggleCollision(lua_State *L, const char *metaTableName)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metaTableName));
    node->toggleCollision();
    return 0;
}

LUA_API int luau_CollisionShape2DToggleCollision(lua_State *L)
{
    return luau_TemplateCollisionShape2DToggleCollision<CollisionShape2D>(L, "CollisionShape2DMetaTable");
}

LUA_API int luau_Area2DToggleCollision(lua_State *L)
{
    return luau_TemplateCollisionShape2DToggleCollision<Area2D>(L, "Area2DMetaTable");
}

LUA_API int luau_RigidBody2DToggleCollision(lua_State *L)
{
    return luau_TemplateCollisionShape2DToggleCollision<RigidBody2D>(L, "RigidBody2DMetaTable");
}

LUA_API int luau_StaticBody2DToggleCollision(lua_State *L)
{
    return luau_TemplateCollisionShape2DToggleCollision<StaticBody2D>(L, "StaticBody2DMetaTable");
}

/** ToggleCollision functions **/

/** IsCollisionEnabled functions **/

template <typename T>
LUA_API int luau_TemplateCollisionShape2DIsCollisionEnabled(lua_State *L, const char *metaTableName)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metaTableName));
    lua_pushboolean(L, node->isCollisionEnabled());
    return 1;
}

LUA_API int luau_CollisionShape2DIsCollisionEnabled(lua_State *L)
{
    return luau_TemplateCollisionShape2DIsCollisionEnabled<CollisionShape2D>(L, "CollisionShape2DMetaTable");
}

LUA_API int luau_Area2DIsCollisionEnabled(lua_State *L)
{
    return luau_TemplateCollisionShape2DIsCollisionEnabled<Area2D>(L, "Area2DMetaTable");
}

LUA_API int luau_RigidBody2DIsCollisionEnabled(lua_State *L)
{
    return luau_TemplateCollisionShape2DIsCollisionEnabled<RigidBody2D>(L, "RigidBody2DMetaTable");
}

LUA_API int luau_StaticBody2DIsCollisionEnabled(lua_State *L)
{
    return luau_TemplateCollisionShape2DIsCollisionEnabled<StaticBody2D>(L, "StaticBody2DMetaTable");
}

/** IsCollisionEnabled functions **/

/** CollisionShape2D functions **/

LUA_API int luau_CollisionShape2DGetBoundingBox(lua_State *L)
{
    CollisionShape2D *node = *static_cast<CollisionShape2D **>(luaL_checkudata(L, 1, "CollisionShape2DMetaTable"));
    Types::Rect2 boundingBox = node->getBoundingBox();
    lua_pushnumber(L, boundingBox.origin.x);
    lua_pushnumber(L, boundingBox.origin.y);
    lua_pushnumber(L, boundingBox.size.x);
    lua_pushnumber(L, boundingBox.size.y);
    return 4;
}

/** CollisionShape2D functions **/

/** Area2D functions **/

LUA_API int luau_Area2DGetSize(lua_State *L)
{
    Area2D *node = *static_cast<Area2D **>(luaL_checkudata(L, 1, "Area2DMetaTable"));
    lua_pushnumber(L, node->getSize().x);
    lua_pushnumber(L, node->getSize().y);
    return 2;
}

LUA_API int luau_Area2DSetSize(lua_State *L)
{
    Area2D *node = *static_cast<Area2D **>(luaL_checkudata(L, 1, "Area2DMetaTable"));
    node->setSize(Types::Vector2(luaL_checknumber(L, 2), luaL_checknumber(L, 3)));
    return 0;
}

/** Area2D functions **/

/** Parallax functions **/

LUA_API int luau_ParallaxSetReferenceNode(lua_State *L)
{
    Parallax *node = *static_cast<Parallax **>(luaL_checkudata(L, 1, "ParallaxMetaTable"));
    Node2D *referenceNode = *static_cast<Node2D **>(luaL_checkudata(L, 2, "Node2DMetaTable"));
    node->setReferenceNode(referenceNode);
    return 0;
}

LUA_API int luau_ParallaxAddParallaxPosition(lua_State *L)
{
    Parallax *node = *static_cast<Parallax **>(luaL_checkudata(L, 1, "ParallaxMetaTable"));
    double x = luaL_checknumber(L, 2);
    double y = luaL_checknumber(L, 3);
    node->addParallaxPosition(Types::Vector2(x, y));
    return 0;
}

/** Parallax functions **/


/* NODE LIBRARY */

/* LUA API LIBRARY */

void luau_ExposeGlobalFunction(lua_State *L, const lua_CFunction func, const char *name)
{
    lua_pushcfunction(L, func, name);
    lua_setglobal(L, name);
}

void luau_ExposeFunctionsAsLibrary(lua_State *L, const luaL_Reg *functions, const char *name)
{
    lua_newtable(L);
    luaL_register(L, name, functions);
    lua_newtable(L);
    lua_pushstring(L, "__newindex");
    lua_pushcfunction(
        L,
        [](lua_State *Lm) -> int
        {
            luaL_error(Lm, "attempt to modify a read-only table");
            return 0;
        },
        name);
    lua_settable(L, -3);

    lua_pushstring(L, "__metatable");
    lua_pushstring(L, "protected");
    lua_settable(L, -3);

    lua_setmetatable(L, -2);

    lua_setglobal(L, name);
}

void luau_ExposeFunctionsAsMetatable(lua_State *L, const luaL_Reg *functions, const char *name)
{
    luaL_newmetatable(L, name);

    lua_pushstring(L, "__index");
    lua_newtable(L);

    lua_pushstring(L, "__name");
    lua_pushstring(L, name);
    lua_settable(L, -3);

    for (const luaL_Reg *func = functions; func->name; func++) {
        lua_pushstring(L, func->name);
        lua_pushcfunction(L, func->func, func->name);
        lua_settable(L, -3);
    }

    lua_settable(L, -3);
    lua_pop(L, 1);
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
    lua_pushcfunction(
        L,
        [](lua_State *Lm) -> int
        {
            luaL_error(Lm, "attempt to modify a read-only table");
            return 0;
        },
        "game");
    lua_settable(L, -3);

    lua_setmetatable(L, -2);

    lua_setglobal(L, "game");
}

void luau_ExposeConstants(lua_State *L, const Types::VMState state)
{
    lua_pushboolean(L, state == Types::VMState::CLIENT);
    lua_setglobal(L, "CLIENT");

    lua_pushboolean(L, state == Types::VMState::SERVER);
    lua_setglobal(L, "SERVER");

#ifdef RTYPE_DEBUG
    lua_pushinteger(L, RTYPE_DEBUG);
    lua_setglobal(L, "DEBUG");
#endif
}

void luau_ExposeFunctions(lua_State *L)
{
    luau_ExposeGlobalFunction(L, luau_Include, "include");
    luau_ExposeGlobalFunction(L, luau_Info, "info");
    luau_ExposeGlobalFunction(L, luau_Warn, "warn");
    luau_ExposeGlobalFunction(L, luau_Error, "error");
    luau_ExposeGlobalFunction(L, luau_Debug, "debug");

    /* NODE LIBRARY */
    constexpr luaL_Reg nodeLibrary[] = {{"GetName", luau_NodeGetName},
                                        {"SetName", luau_NodeSetName},
                                        {"GetChildren", luau_NodeGetChildren},
                                        {"GetChild", luau_NodeGetChild},
                                        {"AddChild", luau_NodeAddChild},
                                        {"CreateChild", luau_NodeCreateChild},
                                        {"__gc", lua_gcNode},
                                        {nullptr, nullptr}};
    luau_ExposeFunctionsAsMetatable(L, nodeLibrary, "NodeMetaTable");
    constexpr luaL_Reg node2DLibrary[] = {{"GetName", luau_Node2DGetName},
                                          {"SetName", luau_Node2DSetName},
                                          {"GetChildren", luau_Node2DGetChildren},
                                          {"GetChild", luau_Node2DGetChild},
                                          {"AddChild", luau_Node2DAddChild},
                                          {"GetPosition", luau_Node2DGetPosition},
                                          {"SetPosition", luau_Node2DSetPosition},
                                          {"CreateChild", luau_Node2DCreateChild},
                                          {"Update", luau_Node2DUpdate},
                                          {"__gc", lua_gcNode2D},
                                          {nullptr, nullptr}};
    luau_ExposeFunctionsAsMetatable(L, node2DLibrary, "Node2DMetaTable");
    constexpr luaL_Reg sprite2DLibrary[] = {{"GetName", luau_Sprite2DGetName},
                                            {"SetName", luau_Sprite2DSetName},
                                            {"GetChildren", luau_Sprite2DGetChildren},
                                            {"GetChild", luau_Sprite2DGetChild},
                                            {"AddChild", luau_Sprite2DAddChild},
                                            {"GetPosition", luau_Sprite2DGetPosition},
                                            {"SetPosition", luau_Sprite2DSetPosition},
                                            {"CreateChild", luau_Sprite2DCreateChild},
                                            {"Update", luau_Sprite2DUpdate},
                                            {"GetPosition", luau_Sprite2DGetPosition},
                                            {"SetPosition", luau_Sprite2DSetPosition},
                                            {"SetTexture", luau_Sprite2DSetTexture},
                                            {"SetSource", luau_Sprite2DSetSource},
                                            {"Draw", luau_Sprite2DDraw},
                                            {"__gc", lua_gcSprite2D},
                                            {nullptr, nullptr}};
    luau_ExposeFunctionsAsMetatable(L, sprite2DLibrary, "Sprite2DMetaTable");
    constexpr luaL_Reg collisionshape2DLibrary[] = {{"GetName", luau_CollisionShape2DGetName},
                                                    {"SetName", luau_CollisionShape2DSetName},
                                                    {"GetChildren", luau_CollisionShape2DGetChildren},
                                                    {"GetChild", luau_CollisionShape2DGetChild},
                                                    {"AddChild", luau_CollisionShape2DAddChild},
                                                    {"GetPosition", luau_CollisionShape2DGetPosition},
                                                    {"SetPosition", luau_CollisionShape2DSetPosition},
                                                    {"CreateChild", luau_CollisionShape2DCreateChild},
                                                    {"Update", luau_CollisionShape2DUpdate},
                                                    {"GetBoundingBox", luau_CollisionShape2DGetBoundingBox},
                                                    {"ToggleCollision", luau_CollisionShape2DToggleCollision},
                                                    {"IsCollisionEnabled", luau_CollisionShape2DIsCollisionEnabled},
                                                    {"Collide", luau_CollisionShape2DCollide},
                                                    {"__gc", lua_gcCollisionShape2D},
                                                    {nullptr, nullptr}};
    luau_ExposeFunctionsAsMetatable(L, collisionshape2DLibrary, "CollisionShape2DMetaTable");
    constexpr luaL_Reg area2DLibrary[] = {{"GetName", luau_Area2DGetName},
                                          {"SetName", luau_Area2DSetName},
                                          {"GetChildren", luau_Area2DGetChildren},
                                          {"GetChild", luau_Area2DGetChild},
                                          {"AddChild", luau_Area2DAddChild},
                                          {"GetPosition", luau_Area2DGetPosition},
                                          {"SetPosition", luau_Area2DSetPosition},
                                          {"CreateChild", luau_Area2DCreateChild},
                                          {"Update", luau_Area2DUpdate},
                                          {"ToggleCollision", luau_Area2DToggleCollision},
                                          {"IsCollisionEnabled", luau_Area2DIsCollisionEnabled},
                                          {"Collide", luau_Area2DCollide},
                                          {"GetSize", luau_Area2DGetSize},
                                          {"SetSize", luau_Area2DSetSize},
                                          {"__gc", lua_gcArea2D},
                                          {nullptr, nullptr}};
    luau_ExposeFunctionsAsMetatable(L, area2DLibrary, "Area2DMetaTable");
    constexpr luaL_Reg parallaxLibrary[] = {{"GetName", luau_ParallaxGetName},
                                            {"SetName", luau_ParallaxSetName},
                                            {"GetChildren", luau_ParallaxGetChildren},
                                            {"GetChild", luau_ParallaxGetChild},
                                            {"AddChild", luau_ParallaxAddChild},
                                            {"Update", luau_ParallaxUpdate},
                                            {"CreateChild", luau_ParallaxCreateChild},
                                            {"SetReferenceNode", luau_ParallaxSetReferenceNode},
                                            {"AddParallaxPosition", luau_ParallaxAddParallaxPosition},
                                            {"__gc", lua_gcParallax},
                                            {nullptr, nullptr}};
    luau_ExposeFunctionsAsMetatable(L, parallaxLibrary, "ParallaxMetaTable");
    /* NODE LIBRARY */
}

/* LUA API LIBRARY */
