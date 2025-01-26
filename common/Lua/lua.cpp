//
// Created by Théo Bary on 29/11/2024.
//

#include "lua.hpp"
#include "Nodes/UI/Box/Box.hpp"
#include "Nodes/UI/Label/Label.hpp"
#include "Nodes/UI/Parallax/Parallax.hpp"
#include "luaconf.h"
#include <cstring>
#include <random>
#include <stdexcept>

#include <Nodes/Node.hpp>
#include <Nodes/Node2D/CollisionNode2D/Area2D/Area2D.hpp>
#include <Nodes/Node2D/CollisionNode2D/Body2D/RigidBody2D/RigidBody2D.hpp>
#include <Nodes/Node2D/CollisionNode2D/Body2D/StaticBody2D/StaticBody2D.hpp>
#include <Nodes/Node2D/CollisionShape2D/CollisionShape2D.hpp>
#include <Nodes/Node2D/Node2D.hpp>
#include <Nodes/Node2D/Sprite2D/Sprite2D.hpp>
#include <Nodes/Shape2D/Rectangle2D/Rectangle2D.hpp>
#include <Nodes/Sound/SoundPlayer.hpp>

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

LUA_API int luau_NetWriteBool(lua_State *L)
{
    const bool b = lua_toboolean(L, 1);
    PacketBuilder &builder = Engine::GetInstance().getPacketBuilders().top();
    builder.writeBool(b);
    return lua_gettop(L);
}

LUA_API int luau_NetReadBool(lua_State *L)
{
    PacketBuilder &builder = Engine::GetInstance().getPacketBuilders().top();
    const bool b = builder.readBool();
    lua_pushboolean(L, b);
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

/** Factory functions **/

static Node *luau_FindNodeFactory(lua_State *L, int pos)
{
    Node *node = nullptr;
    const char *metatables[] = {"NodeMetaTable", "Node2DMetaTable", "CollisionShape2DMetaTable", "Sprite2DMetaTable",
                                 "Area2DMetaTable", "ParallaxMetaTable", "RigidBody2DMetaTable", "StaticBody2DMetaTable",
                                 "LabelMetaTable", "BoxMetaTable", "SoundPlayerMetaTable"};
    for (const char *metatable : metatables) {
        try {
            node = *static_cast<Node **>(luaL_checkudata(L, pos, metatable));
            break;
        } catch (const std::exception &) {}
    }
    if (node == nullptr) {
        luaL_error(L, "Node type not found.");
    }
    return node;
}

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
        double width = luaL_checknumber(L, 6);
        double height = luaL_checknumber(L, 7);
        child = new StaticBody2D(name, Types::Vector2(x, y), Types::Vector2(width, height));
    }
    else if (type == "RigidBody2D") {
        double x = luaL_checknumber(L, 4);
        double y = luaL_checknumber(L, 5);
        double width = luaL_checknumber(L, 6);
        double height = luaL_checknumber(L, 7);
        double vel_x = luaL_checknumber(L, 8);
        double vel_y = luaL_checknumber(L, 9);
        child = new RigidBody2D(name, Types::Vector2(x, y), Types::Vector2(width, height), Types::Vector2(vel_x, vel_y));
    }
    else if (type == "Label") {
        double x = luaL_checknumber(L, 4);
        double y = luaL_checknumber(L, 5);
        const char *text = luaL_checkstring(L, 6);
        const char *font = luaL_checkstring(L, 7);
        int fontSize = luaL_checkinteger(L, 8);
        child = new Label(name, Types::Vector2(x, y), text, font, fontSize);
    }
    else if (type == "Box") {
        double x = luaL_checknumber(L, 4);
        double y = luaL_checknumber(L, 5);
        double size_x = luaL_checknumber(L, 6);
        double size_y = luaL_checknumber(L, 7);
        child = new Box(name, Types::Vector2(x, y), Types::Vector2(size_x, size_y));
    }
    else if (type == "SoundPlayer") {
        const char *sound = luaL_checkstring(L, 4);
        child = new SoundPlayer(name, sound);
    }
    else {
        luaL_error(L, "Invalid type '%s' provided to AddChild in Node.", type.c_str());
    }
    return child;
}


/** Factory functions **/


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

LUA_API int lua_gcLabel(lua_State *L)
{
    Label *label = *static_cast<Label **>(luaL_checkudata(L, 1, "LabelMetaTable"));
    delete label;
    return 0;
}

LUA_API int lua_gcBox(lua_State *L)
{
    Box *box = *static_cast<Box **>(luaL_checkudata(L, 1, "BoxMetaTable"));
    delete box;
    return 0;
}

LUA_API int lua_gcSoundPlayer(lua_State *L)
{
    SoundPlayer *soundPlayer = *static_cast<SoundPlayer **>(luaL_checkudata(L, 1, "SoundPlayerMetaTable"));
    delete soundPlayer;
    return 0;
}

/** __gc functions **/

/** Destroy functions **/

template <typename T>
LUA_API int luau_TemplateNodeDestroy(lua_State *L, const char *metaTableName)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metaTableName));
    node->Destroy();
    return 0;
}

LUA_API int luau_NodeDestroy(lua_State *L) { return luau_TemplateNodeDestroy<Node>(L, "NodeMetaTable"); }

LUA_API int luau_Node2DDestroy(lua_State *L) { return luau_TemplateNodeDestroy<Node2D>(L, "Node2DMetaTable"); }

LUA_API int luau_CollisionShape2DDestroy(lua_State *L)
{
    return luau_TemplateNodeDestroy<CollisionShape2D>(L, "CollisionShape2DMetaTable");
}

LUA_API int luau_Sprite2DDestroy(lua_State *L) { return luau_TemplateNodeDestroy<Sprite2D>(L, "Sprite2DMetaTable"); }

LUA_API int luau_Area2DDestroy(lua_State *L) { return luau_TemplateNodeDestroy<Area2D>(L, "Area2DMetaTable"); }

LUA_API int luau_ParallaxDestroy(lua_State *L) { return luau_TemplateNodeDestroy<Parallax>(L, "ParallaxMetaTable"); }

LUA_API int luau_RigidBody2DDestroy(lua_State *L)
{
    return luau_TemplateNodeDestroy<RigidBody2D>(L, "RigidBody2DMetaTable");
}

LUA_API int luau_StaticBody2DDestroy(lua_State *L)
{
    return luau_TemplateNodeDestroy<StaticBody2D>(L, "StaticBody2DMetaTable");
}

LUA_API int luau_LabelDestroy(lua_State *L) { return luau_TemplateNodeDestroy<Label>(L, "LabelMetaTable"); }

LUA_API int luau_BoxDestroy(lua_State *L) { return luau_TemplateNodeDestroy<Box>(L, "BoxMetaTable"); }

LUA_API int luau_SoundPlayerDestroy(lua_State *L) { return luau_TemplateNodeDestroy<SoundPlayer>(L, "SoundPlayerMetaTable"); }

/** Destroy functions **/

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

LUA_API int luau_LabelGetName(lua_State *L) { return luau_TemplateNodeGetName<Label>(L, "LabelMetaTable"); }

LUA_API int luau_BoxGetName(lua_State *L) { return luau_TemplateNodeGetName<Box>(L, "BoxMetaTable"); }

LUA_API int luau_SoundPlayerGetName(lua_State *L) { return luau_TemplateNodeGetName<SoundPlayer>(L, "SoundPlayerMetaTable"); }

/** GetName functions **/

/** SetName functions **/

template <typename T>
LUA_API int luau_TemplateNodeSetName(lua_State *L, const char *metaTableName)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metaTableName));
    const char *name = luaL_checkstring(L, 2);
    if (std::strcmp(name, "root") == 0) {
        luaL_error(L, "Cannot set node name to 'root'.");
    }
    node->name = name;
    return 0;
}

LUA_API int luau_NodeSetName(lua_State *L) { return luau_TemplateNodeSetName<Node>(L, "NodeMetaTable"); }

LUA_API int luau_Node2DSetName(lua_State *L) { return luau_TemplateNodeSetName<Node2D>(L, "Node2DMetaTable"); }

LUA_API int luau_CollisionShape2DSetName(lua_State *L)
{
    return luau_TemplateNodeSetName<CollisionShape2D>(L, "CollisionShape2DMetaTable");
}

LUA_API int luau_Sprite2DSetName(lua_State *L) { return luau_TemplateNodeSetName<Sprite2D>(L, "Sprite2DMetaTable"); }

LUA_API int luau_Area2DSetName(lua_State *L) { return luau_TemplateNodeSetName<Area2D>(L, "Area2DMetaTable"); }

LUA_API int luau_ParallaxSetName(lua_State *L) { return luau_TemplateNodeSetName<Parallax>(L, "ParallaxMetaTable"); }

LUA_API int luau_RigidBody2DSetName(lua_State *L)
{
    return luau_TemplateNodeSetName<RigidBody2D>(L, "RigidBody2DMetaTable");
}

LUA_API int luau_StaticBody2DSetName(lua_State *L)
{
    return luau_TemplateNodeSetName<StaticBody2D>(L, "StaticBody2DMetaTable");
}

LUA_API int luau_LabelSetName(lua_State *L) { return luau_TemplateNodeSetName<Label>(L, "LabelMetaTable"); }

LUA_API int luau_BoxSetName(lua_State *L) { return luau_TemplateNodeSetName<Box>(L, "BoxMetaTable"); }

LUA_API int luau_SoundPlayerSetName(lua_State *L)
{
    return luau_TemplateNodeSetName<SoundPlayer>(L, "SoundPlayerMetaTable");
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

LUA_API int luau_LabelGetChildren(lua_State *L) { return luau_TemplateNodeGetChildren<Label>(L, "LabelMetaTable"); }

LUA_API int luau_BoxGetChildren(lua_State *L) { return luau_TemplateNodeGetChildren<Box>(L, "BoxMetaTable"); }

LUA_API int luau_SoundPlayerGetChildren(lua_State *L)
{
    return luau_TemplateNodeGetChildren<SoundPlayer>(L, "SoundPlayerMetaTable");
}

/** GetChildren functions **/

/** GetChild functions **/

template <typename T>
LUA_API int luau_TemplateGetChild(lua_State *L, const char *metatable)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metatable));
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

LUA_API int luau_NodeGetChild(lua_State *L) { return luau_TemplateGetChild<Node>(L, "NodeMetaTable"); }

LUA_API int luau_Node2DGetChild(lua_State *L) { return luau_TemplateGetChild<Node2D>(L, "Node2DMetaTable"); }

LUA_API int luau_CollisionShape2DGetChild(lua_State *L)
{
    return luau_TemplateGetChild<CollisionShape2D>(L, "CollisionShape2DMetaTable");
}

LUA_API int luau_Sprite2DGetChild(lua_State *L) { return luau_TemplateGetChild<Sprite2D>(L, "Sprite2DMetaTable"); }

LUA_API int luau_Area2DGetChild(lua_State *L) { return luau_TemplateGetChild<Area2D>(L, "Area2DMetaTable"); }

LUA_API int luau_ParallaxGetChild(lua_State *L) { return luau_TemplateGetChild<Parallax>(L, "ParallaxMetaTable"); }

LUA_API int luau_RigidBody2DGetChild(lua_State *L)
{
    return luau_TemplateGetChild<RigidBody2D>(L, "RigidBody2DMetaTable");
}

LUA_API int luau_StaticBody2DGetChild(lua_State *L)
{
    return luau_TemplateGetChild<StaticBody2D>(L, "StaticBody2DMetaTable");
}

LUA_API int luau_LabelGetChild(lua_State *L) { return luau_TemplateGetChild<Label>(L, "LabelMetaTable"); }

LUA_API int luau_BoxGetChild(lua_State *L) { return luau_TemplateGetChild<Box>(L, "BoxMetaTable"); }

LUA_API int luau_SoundPlayerGetChild(lua_State *L)
{
    return luau_TemplateGetChild<SoundPlayer>(L, "SoundPlayerMetaTable");
}

/** GetChild functions **/

/** CreateChild functions **/

template <typename T>
LUA_API int luau_TemplateCreateChild(lua_State *L, const char *metatable)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metatable));
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
    try {
        node->addChild(*child);
    }
    catch (const std::runtime_error &e) {
        luaL_error(L, e.what());
    }
    return 1;
}

LUA_API int luau_NodeCreateChild(lua_State *L) { return luau_TemplateCreateChild<Node>(L, "NodeMetaTable"); }

LUA_API int luau_Node2DCreateChild(lua_State *L) { return luau_TemplateCreateChild<Node2D>(L, "Node2DMetaTable"); }

LUA_API int luau_CollisionShape2DCreateChild(lua_State *L)
{
    return luau_TemplateCreateChild<CollisionShape2D>(L, "CollisionShape2DMetaTable");
}

LUA_API int luau_Sprite2DCreateChild(lua_State *L)
{
    return luau_TemplateCreateChild<Sprite2D>(L, "Sprite2DMetaTable");
}

LUA_API int luau_Area2DCreateChild(lua_State *L) { return luau_TemplateCreateChild<Area2D>(L, "Area2DMetaTable"); }

LUA_API int luau_ParallaxCreateChild(lua_State *L)
{
    return luau_TemplateCreateChild<Parallax>(L, "ParallaxMetaTable");
}

LUA_API int luau_RigidBody2DCreateChild(lua_State *L)
{
    return luau_TemplateCreateChild<RigidBody2D>(L, "RigidBody2DMetaTable");
}

LUA_API int luau_StaticBody2DCreateChild(lua_State *L)
{
    return luau_TemplateCreateChild<StaticBody2D>(L, "StaticBody2DMetaTable");
}

LUA_API int luau_LabelCreateChild(lua_State *L) { return luau_TemplateCreateChild<Label>(L, "LabelMetaTable"); }

LUA_API int luau_BoxCreateChild(lua_State *L) { return luau_TemplateCreateChild<Box>(L, "BoxMetaTable"); }

LUA_API int luau_SoundPlayerCreateChild(lua_State *L)
{
    return luau_TemplateCreateChild<SoundPlayer>(L, "SoundPlayerMetaTable");
}

/** CreateChild functions **/

/** AddChild functions **/

template <typename T>
LUA_API int luau_TemplateAddChild(lua_State *L, const char *metatable)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metatable));
    Node *child = luau_FindNodeFactory(L, 2);
    try {
        node->addChild(*child);
    }
    catch (const std::runtime_error &e) {
        luaL_error(L, e.what());
    }
    return 0;
}

LUA_API int luau_NodeAddChild(lua_State *L) { return luau_TemplateAddChild<Node>(L, "NodeMetaTable"); }

LUA_API int luau_Node2DAddChild(lua_State *L) { return luau_TemplateAddChild<Node2D>(L, "Node2DMetaTable"); }

LUA_API int luau_CollisionShape2DAddChild(lua_State *L)
{
    return luau_TemplateAddChild<CollisionShape2D>(L, "CollisionShape2DMetaTable");
}

LUA_API int luau_Sprite2DAddChild(lua_State *L) { return luau_TemplateAddChild<Sprite2D>(L, "Sprite2DMetaTable"); }

LUA_API int luau_Area2DAddChild(lua_State *L) { return luau_TemplateAddChild<Area2D>(L, "Area2DMetaTable"); }

LUA_API int luau_ParallaxAddChild(lua_State *L) { return luau_TemplateAddChild<Parallax>(L, "ParallaxMetaTable"); }

LUA_API int luau_RigidBody2DAddChild(lua_State *L)
{
    return luau_TemplateAddChild<RigidBody2D>(L, "RigidBody2DMetaTable");
}

LUA_API int luau_StaticBody2DAddChild(lua_State *L)
{
    return luau_TemplateAddChild<StaticBody2D>(L, "StaticBody2DMetaTable");
}

LUA_API int luau_LabelAddChild(lua_State *L) { return luau_TemplateAddChild<Label>(L, "LabelMetaTable"); }

LUA_API int luau_BoxAddChild(lua_State *L) { return luau_TemplateAddChild<Box>(L, "BoxMetaTable"); }

LUA_API int luau_SoundPlayerAddChild(lua_State *L)
{
    return luau_TemplateAddChild<SoundPlayer>(L, "SoundPlayerMetaTable");
}

/** AddChild functions **/

/** GetPosition functions **/

template <typename T>
LUA_API int luau_TemplateNode2DGetPosition(lua_State *L, const char *metaTableName)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metaTableName));
    lua_pushnumber(L, node->position.x);
    lua_pushnumber(L, node->position.y);
    return 2;
}

LUA_API int luau_Node2DGetPosition(lua_State *L)
{
    return luau_TemplateNode2DGetPosition<Node2D>(L, "Node2DMetaTable");
}

LUA_API int luau_CollisionShape2DGetPosition(lua_State *L)
{
    return luau_TemplateNode2DGetPosition<CollisionShape2D>(L, "CollisionShape2DMetaTable");
}

LUA_API int luau_Sprite2DGetPosition(lua_State *L)
{
    return luau_TemplateNode2DGetPosition<Sprite2D>(L, "Sprite2DMetaTable");
}

LUA_API int luau_Area2DGetPosition(lua_State *L)
{
    return luau_TemplateNode2DGetPosition<Area2D>(L, "Area2DMetaTable");
}

LUA_API int luau_RigidBody2DGetPosition(lua_State *L)
{
    return luau_TemplateNode2DGetPosition<RigidBody2D>(L, "RigidBody2DMetaTable");
}

LUA_API int luau_StaticBody2DGetPosition(lua_State *L)
{
    return luau_TemplateNode2DGetPosition<StaticBody2D>(L, "StaticBody2DMetaTable");
}

LUA_API int luau_LabelGetPosition(lua_State *L) { return luau_TemplateNode2DGetPosition<Label>(L, "LabelMetaTable"); }

LUA_API int luau_BoxGetPosition(lua_State *L) { return luau_TemplateNode2DGetPosition<Box>(L, "BoxMetaTable"); }

/** GetPosition functions **/

/** SetPosition functions **/

template <typename T>
LUA_API int luau_TemplateNode2DSetPosition(lua_State *L, const char *metaTableName)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metaTableName));
    node->position.x = static_cast<float>(luaL_checknumber(L, 2));
    node->position.y = static_cast<float>(luaL_checknumber(L, 3));
    return 0;
}

LUA_API int luau_Node2DSetPosition(lua_State *L)
{
    return luau_TemplateNode2DSetPosition<Node2D>(L, "Node2DMetaTable");
}

LUA_API int luau_CollisionShape2DSetPosition(lua_State *L)
{
    return luau_TemplateNode2DSetPosition<CollisionShape2D>(L, "CollisionShape2DMetaTable");
}

LUA_API int luau_Sprite2DSetPosition(lua_State *L)
{
    return luau_TemplateNode2DSetPosition<Sprite2D>(L, "Sprite2DMetaTable");
}

LUA_API int luau_Area2DSetPosition(lua_State *L)
{
    return luau_TemplateNode2DSetPosition<Area2D>(L, "Area2DMetaTable");
}

LUA_API int luau_RigidBody2DSetPosition(lua_State *L)
{
    return luau_TemplateNode2DSetPosition<RigidBody2D>(L, "RigidBody2DMetaTable");
}

LUA_API int luau_StaticBody2DSetPosition(lua_State *L)
{
    return luau_TemplateNode2DSetPosition<StaticBody2D>(L, "StaticBody2DMetaTable");
}

LUA_API int luau_LabelSetPosition(lua_State *L) { return luau_TemplateNode2DSetPosition<Label>(L, "LabelMetaTable"); }

LUA_API int luau_BoxSetPosition(lua_State *L) { return luau_TemplateNode2DSetPosition<Box>(L, "BoxMetaTable"); }

/** SetPosition functions **/

/** GetGlobalPosition functions **/

template<typename T>
LUA_API int luau_TemplateNode2DGetGlobalPosition(lua_State *L, const char *metaTableName)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metaTableName));
    Types::Vector2 globalPosition = node->getGlobalPosition();
    lua_pushnumber(L, globalPosition.x);
    lua_pushnumber(L, globalPosition.y);
    return 2;
}

LUA_API int luau_Node2DGetGlobalPosition(lua_State *L)
{
    return luau_TemplateNode2DGetGlobalPosition<Node2D>(L, "Node2DMetaTable");
}

LUA_API int luau_CollisionShape2DGetGlobalPosition(lua_State *L)
{
    return luau_TemplateNode2DGetGlobalPosition<CollisionShape2D>(L, "CollisionShape2DMetaTable");
}

LUA_API int luau_Sprite2DGetGlobalPosition(lua_State *L)
{
    return luau_TemplateNode2DGetGlobalPosition<Sprite2D>(L, "Sprite2DMetaTable");
}

LUA_API int luau_Area2DGetGlobalPosition(lua_State *L)
{
    return luau_TemplateNode2DGetGlobalPosition<Area2D>(L, "Area2DMetaTable");
}

LUA_API int luau_RigidBody2DGetGlobalPosition(lua_State *L)
{
    return luau_TemplateNode2DGetGlobalPosition<RigidBody2D>(L, "RigidBody2DMetaTable");
}

LUA_API int luau_StaticBody2DGetGlobalPosition(lua_State *L)
{
    return luau_TemplateNode2DGetGlobalPosition<StaticBody2D>(L, "StaticBody2DMetaTable");
}

/** GetGlobalPosition functions **/

/** Sprite2D functions **/

LUA_API int luau_Sprite2DSetSize(lua_State *L)
{
    Sprite2D *sprite = *static_cast<Sprite2D **>(luaL_checkudata(L, 1, "Sprite2DMetaTable"));
    float x = static_cast<float>(luaL_checknumber(L, 2));
    float y = static_cast<float>(luaL_checknumber(L, 3));
    sprite->SetSize(Types::Vector2(x, y));
    return 0;
}

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

LUA_API int luau_Sprite2DSetRotation(lua_State *L)
{
    Sprite2D *sprite = *static_cast<Sprite2D **>(luaL_checkudata(L, 1, "Sprite2DMetaTable"));
    double rotation = luaL_checknumber(L, 2);
    sprite->setRotation(static_cast<float>(rotation));
    return 0;
}

LUA_API int luau_Sprite2DGetRotation(lua_State *L)
{
    Sprite2D *sprite = *static_cast<Sprite2D **>(luaL_checkudata(L, 1, "Sprite2DMetaTable"));
    lua_pushnumber(L, sprite->getRotation());
    return 1;
}

/** Sprite2D functions **/

/** Collide functions **/

template <typename T>
LUA_API int luau_TemplateCollide(lua_State *L, const char *metaTableName)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metaTableName));
    Node *other = luau_FindNodeFactory(L, 2);

    // Check if both are CollisionNode2D
    if (auto *collisionNode1 = dynamic_cast<CollisionNode2D *>(node)) {
        if (auto *collisionNode2 = dynamic_cast<CollisionNode2D *>(other)) {
            lua_pushboolean(L, collisionNode1->collidesWith(*collisionNode2));
            return 1;
        }
    }
    // Check if both are CollisionShape2D
    if (auto *collisionShape1 = dynamic_cast<CollisionShape2D *>(node)) {
        if (auto *collisionShape2 = dynamic_cast<CollisionShape2D *>(other)) {
            lua_pushboolean(L, collisionShape1->collidesWith(*collisionShape2));
            return 1;
        }
    }
    // Handle invalid cases
    luaL_error(L, "Invalid node type provided to Collide.");
    return 1;
}

LUA_API int luau_CollisionShape2DCollide(lua_State *L)
{
    return luau_TemplateCollide<CollisionShape2D>(L, "CollisionShape2DMetaTable");
}

LUA_API int luau_Area2DCollide(lua_State *L)
{
    return luau_TemplateCollide<Area2D>(L, "Area2DMetaTable");
}

LUA_API int luau_RigidBody2DCollide(lua_State *L)
{
    return luau_TemplateCollide<RigidBody2D>(L, "RigidBody2DMetaTable");
}

LUA_API int luau_StaticBody2DCollide(lua_State *L)
{
    return luau_TemplateCollide<StaticBody2D>(L, "StaticBody2DMetaTable");
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

/** RigidBody2D functions **/

LUA_API int luau_RigidBody2DSetVelocity(lua_State *L)
{
    RigidBody2D *node = *static_cast<RigidBody2D **>(luaL_checkudata(L, 1, "RigidBody2DMetaTable"));
    const double x = luaL_checknumber(L, 2);
    const double y = luaL_checknumber(L, 3);
    node->velocity.x = static_cast<float>(x);
    node->velocity.y = static_cast<float>(y);
    return 0;
}

LUA_API int luau_RigidBody2DGetVelocity(lua_State *L)
{
    RigidBody2D *node = *static_cast<RigidBody2D **>(luaL_checkudata(L, 1, "RigidBody2DMetaTable"));
    lua_pushnumber(L, node->velocity.x);
    lua_pushnumber(L, node->velocity.y);
    return 2;
}

/** RigidBody2D functions **/

/** SetColor functions **/

template <typename T>
LUA_API int luau_TemplateSetColor(lua_State *L, const char *metaTableName)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metaTableName));
    Types::Vector3 color = Types::Vector3(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
    node->setRGB(color);
    return 0;
}

LUA_API int luau_LabelSetColor(lua_State *L) { return luau_TemplateSetColor<Label>(L, "LabelMetaTable"); }

LUA_API int luau_BoxSetColor(lua_State *L) { return luau_TemplateSetColor<Box>(L, "BoxMetaTable"); }

/** SetColor functions **/

/** SetAlpha functions **/

template <typename T>
LUA_API int luau_TemplateSetAlpha(lua_State *L, const char *metaTableName)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metaTableName));
    node->setAlpha(luaL_checknumber(L, 2));
    return 0;
}

LUA_API int luau_LabelSetAlpha(lua_State *L) { return luau_TemplateSetAlpha<Label>(L, "LabelMetaTable"); }

LUA_API int luau_BoxSetAlpha(lua_State *L) { return luau_TemplateSetAlpha<Box>(L, "BoxMetaTable"); }

/** SetAlpha functions **/

/** GetColor functions **/

template <typename T>
LUA_API int luau_TemplateGetColor(lua_State *L, const char *metaTableName)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metaTableName));
    Types::Vector3 color = node->getRGB();
    lua_pushnumber(L, color.x);
    lua_pushnumber(L, color.y);
    lua_pushnumber(L, color.z);
    return 3;
}

LUA_API int luau_LabelGetColor(lua_State *L) { return luau_TemplateGetColor<Label>(L, "LabelMetaTable"); }

LUA_API int luau_BoxGetColor(lua_State *L) { return luau_TemplateGetColor<Box>(L, "BoxMetaTable"); }

/** GetColor functions **/

/** GetAlpha functions **/

template <typename T>
LUA_API int luau_TemplateGetAlpha(lua_State *L, const char *metaTableName)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metaTableName));
    lua_pushnumber(L, node->getAlpha());
    return 1;
}

LUA_API int luau_LabelGetAlpha(lua_State *L) { return luau_TemplateGetAlpha<Label>(L, "LabelMetaTable"); }

LUA_API int luau_BoxGetAlpha(lua_State *L) { return luau_TemplateGetAlpha<Box>(L, "BoxMetaTable"); }

/** GetAlpha functions **/

/** Label functions **/

LUA_API int luau_LabelSetText(lua_State *L)
{
    Label *node = *static_cast<Label **>(luaL_checkudata(L, 1, "LabelMetaTable"));
    node->setText(luaL_checkstring(L, 2));
    return 0;
}

LUA_API int luau_LabelSetFont(lua_State *L)
{
    Label *node = *static_cast<Label **>(luaL_checkudata(L, 1, "LabelMetaTable"));
    node->setFont(luaL_checkstring(L, 2));
    return 0;
}

LUA_API int luau_LabelSetFontSize(lua_State *L)
{
    Label *node = *static_cast<Label **>(luaL_checkudata(L, 1, "LabelMetaTable"));
    node->setFontSize(luaL_checkinteger(L, 2));
    return 0;
}

LUA_API int luau_LabelGetText(lua_State *L)
{
    Label *node = *static_cast<Label **>(luaL_checkudata(L, 1, "LabelMetaTable"));
    lua_pushstring(L, node->getText().c_str());
    return 1;
}

LUA_API int luau_LabelGetFont(lua_State *L)
{
    Label *node = *static_cast<Label **>(luaL_checkudata(L, 1, "LabelMetaTable"));
    lua_pushstring(L, node->getFont().c_str());
    return 1;
}

LUA_API int luau_LabelGetFontSize(lua_State *L)
{
    Label *node = *static_cast<Label **>(luaL_checkudata(L, 1, "LabelMetaTable"));
    lua_pushinteger(L, node->getFontSize());
    return 1;
}

/** Label functions **/

/** Box functions **/

LUA_API int luau_BoxSetSize(lua_State *L)
{
    Box *node = *static_cast<Box **>(luaL_checkudata(L, 1, "BoxMetaTable"));
    node->setSize(Types::Vector2(luaL_checknumber(L, 2), luaL_checknumber(L, 3)));
    return 0;
}

LUA_API int luau_BoxGetSize(lua_State *L)
{
    Box *node = *static_cast<Box **>(luaL_checkudata(L, 1, "BoxMetaTable"));
    lua_pushnumber(L, node->getSize().x);
    lua_pushnumber(L, node->getSize().y);
    return 2;
}

/** Box functions **/

/** GetGlobalPosition functions **/

template <typename T>
LUA_API int luau_TemplateGetGlobalPosition(lua_State *L, const char *metaTableName)
{
    T *node = *static_cast<T **>(luaL_checkudata(L, 1, metaTableName));
    Types::Vector2 globalPosition = node->getGlobalPosition();
    lua_pushnumber(L, globalPosition.x);
    lua_pushnumber(L, globalPosition.y);
    return 2;
}

LUA_API int luau_ParallaxGetGlobalPosition(lua_State *L)
{
    return luau_TemplateGetGlobalPosition<Parallax>(L, "ParallaxMetaTable");
}

LUA_API int luau_LabelGetGlobalPosition(lua_State *L)
{
    return luau_TemplateGetGlobalPosition<Label>(L, "LabelMetaTable");
}

LUA_API int luau_BoxGetGlobalPosition(lua_State *L) { return luau_TemplateGetGlobalPosition<Box>(L, "BoxMetaTable"); }

/** GetGlobalPosition functions **/


/** SoundPlayer functions **/

LUA_API int luau_SoundPlayerPlay(lua_State *L)
{
    SoundPlayer *node = *static_cast<SoundPlayer **>(luaL_checkudata(L, 1, "SoundPlayerMetaTable"));
    node->play();
    return 0;
}

LUA_API int luau_SoundPlayerPause(lua_State *L)
{
    SoundPlayer *node = *static_cast<SoundPlayer **>(luaL_checkudata(L, 1, "SoundPlayerMetaTable"));
    node->pause();
    return 0;
}

LUA_API int luau_SoundPlayerStop(lua_State *L)
{
    SoundPlayer *node = *static_cast<SoundPlayer **>(luaL_checkudata(L, 1, "SoundPlayerMetaTable"));
    node->stop();
    return 0;
}

LUA_API int luau_SoundPlayerSetVolume(lua_State *L)
{
    SoundPlayer *node = *static_cast<SoundPlayer **>(luaL_checkudata(L, 1, "SoundPlayerMetaTable"));
    node->setVolume(luaL_checknumber(L, 2));
    return 0;
}

LUA_API int luau_SoundPlayerSetPitch(lua_State *L)
{
    SoundPlayer *node = *static_cast<SoundPlayer **>(luaL_checkudata(L, 1, "SoundPlayerMetaTable"));
    node->setPitch(luaL_checknumber(L, 2));
    return 0;
}

LUA_API int luau_SoundPlayerSetPan(lua_State *L)
{
    SoundPlayer *node = *static_cast<SoundPlayer **>(luaL_checkudata(L, 1, "SoundPlayerMetaTable"));
    node->setPan(luaL_checknumber(L, 2));
    return 0;
}

LUA_API int luau_SoundPlayerSetSound(lua_State *L)
{
    SoundPlayer *node = *static_cast<SoundPlayer **>(luaL_checkudata(L, 1, "SoundPlayerMetaTable"));
    node->setSound(luaL_checkstring(L, 2));
    return 0;
}

LUA_API int luau_SoundPlayerGetSound(lua_State *L)
{
    SoundPlayer *node = *static_cast<SoundPlayer **>(luaL_checkudata(L, 1, "SoundPlayerMetaTable"));
    lua_pushstring(L, node->getSound().c_str());
    return 1;
}

LUA_API int luau_SoundPlayerResume(lua_State *L)
{
    SoundPlayer *node = *static_cast<SoundPlayer **>(luaL_checkudata(L, 1, "SoundPlayerMetaTable"));
    node->resume();
    return 0;
}

LUA_API int luau_SoundPlayerIsPlaying(lua_State *L)
{
    SoundPlayer *node = *static_cast<SoundPlayer **>(luaL_checkudata(L, 1, "SoundPlayerMetaTable"));
    lua_pushboolean(L, node->isPlaying());
    return 1;
}

LUA_API int luau_SoundPlayerIsStopped(lua_State *L)
{
    SoundPlayer *node = *static_cast<SoundPlayer **>(luaL_checkudata(L, 1, "SoundPlayerMetaTable"));
    lua_pushboolean(L, node->isStopped());
    return 1;
}

/** SoundPlayer functions **/

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

LUA_API int luau_EngineCreateNode(lua_State *L)
{
    const std::string type = luaL_checkstring(L, 2);

    Node *node = luau_NodeFactory(L, type);
    *static_cast<Node **>(lua_newuserdata(L, sizeof(Node *))) = node;
    std::string metatableName = type + "MetaTable";
    luaL_getmetatable(L, metatableName.c_str());
    if (lua_isnil(L, -1)) {
        luaL_error(L, "Metatable '%s' not found. Ensure it is registered before calling AddChild.",
                   metatableName.c_str());
    }
    lua_setmetatable(L, -2);
    return 1;
}

LUA_API int luau_EngineGetRenderWidth(lua_State *L)
{
    lua_pushinteger(L, GetRenderWidth());
    std::cout << "GetRenderWidth: " << GetRenderWidth() << std::endl;
    return 1;
}

LUA_API int luau_EngineGetRenderHeight(lua_State *L)
{
    lua_pushinteger(L, GetRenderHeight());
    return 1;
}

LUA_API int luau_GenerateUUID(lua_State *L)
{
    static std::random_device dev;
    static std::mt19937 rng(dev());

    std::uniform_int_distribution<int> dist(0, 15);

    const char *v = "0123456789ABCDEF";

    std::string str;
    for (int i = 0; i < 16; i++) {
        str += v[dist(rng)];
    }
    lua_pushstring(L, str.c_str());
    return 1;
}

void luau_ExposeFunctions(lua_State *L)
{
    luau_ExposeGlobalFunction(L, luau_Include, "include");
    luau_ExposeGlobalFunction(L, luau_Info, "info");
    luau_ExposeGlobalFunction(L, luau_Warn, "warn");
    luau_ExposeGlobalFunction(L, luau_Error, "error");
    luau_ExposeGlobalFunction(L, luau_Debug, "debug");
    luau_ExposeGlobalFunction(L, luau_GenerateUUID, "generateUUID");

    /* NODE LIBRARY */
    constexpr luaL_Reg nodeRegistry[] = {{"GetName", luau_NodeGetName},
                                        {"SetName", luau_NodeSetName},
                                        {"GetChildren", luau_NodeGetChildren},
                                        {"GetChild", luau_NodeGetChild},
                                        {"AddChild", luau_NodeAddChild},
                                        {"CreateChild", luau_NodeCreateChild},
                                        {"Destroy", luau_NodeDestroy},
                                        {"__gc", lua_gcNode},
                                        {nullptr, nullptr}};
    luau_ExposeFunctionsAsMetatable(L, nodeRegistry, "NodeMetaTable");
    constexpr luaL_Reg node2DRegistry[] = {{"GetName", luau_Node2DGetName},
                                          {"SetName", luau_Node2DSetName},
                                          {"GetChildren", luau_Node2DGetChildren},
                                          {"GetChild", luau_Node2DGetChild},
                                          {"AddChild", luau_Node2DAddChild},
                                          {"GetPosition", luau_Node2DGetPosition},
                                          {"SetPosition", luau_Node2DSetPosition},
                                          {"GetGlobalPosition", luau_Node2DGetGlobalPosition},
                                          {"CreateChild", luau_Node2DCreateChild},
                                          {"Destroy", luau_Node2DDestroy},
                                          {"__gc", lua_gcNode2D},
                                          {nullptr, nullptr}};
    luau_ExposeFunctionsAsMetatable(L, node2DRegistry, "Node2DMetaTable");
    constexpr luaL_Reg sprite2DRegistry[] = {{"GetName", luau_Sprite2DGetName},
                                            {"SetName", luau_Sprite2DSetName},
                                            {"GetChildren", luau_Sprite2DGetChildren},
                                            {"GetChild", luau_Sprite2DGetChild},
                                            {"AddChild", luau_Sprite2DAddChild},
                                            {"GetPosition", luau_Sprite2DGetPosition},
                                            {"SetPosition", luau_Sprite2DSetPosition},
                                            {"GetGlobalPosition", luau_Sprite2DGetGlobalPosition},
                                            {"CreateChild", luau_Sprite2DCreateChild},
                                            {"SetSize", luau_Sprite2DSetSize},
                                            {"SetTexture", luau_Sprite2DSetTexture},
                                            {"SetSource", luau_Sprite2DSetSource},
                                            {"SetRotation", luau_Sprite2DSetRotation},
                                            {"GetRotation", luau_Sprite2DGetRotation},
                                            {"Destroy", luau_Sprite2DDestroy},
                                            {"__gc", lua_gcSprite2D},
                                            {nullptr, nullptr}};
    luau_ExposeFunctionsAsMetatable(L, sprite2DRegistry, "Sprite2DMetaTable");
    constexpr luaL_Reg collisionshape2DRegistry[] = {{"GetName", luau_CollisionShape2DGetName},
                                                    {"SetName", luau_CollisionShape2DSetName},
                                                    {"GetChildren", luau_CollisionShape2DGetChildren},
                                                    {"GetChild", luau_CollisionShape2DGetChild},
                                                    {"AddChild", luau_CollisionShape2DAddChild},
                                                    {"GetPosition", luau_CollisionShape2DGetPosition},
                                                    {"SetPosition", luau_CollisionShape2DSetPosition},
                                                    {"GetGlobalPosition", luau_CollisionShape2DGetGlobalPosition},
                                                    {"CreateChild", luau_CollisionShape2DCreateChild},
                                                    {"GetBoundingBox", luau_CollisionShape2DGetBoundingBox},
                                                    {"ToggleCollision", luau_CollisionShape2DToggleCollision},
                                                    {"IsCollisionEnabled", luau_CollisionShape2DIsCollisionEnabled},
                                                    {"Collide", luau_CollisionShape2DCollide},
                                                    {"Destroy", luau_CollisionShape2DDestroy},
                                                    {"__gc", lua_gcCollisionShape2D},
                                                    {nullptr, nullptr}};
    luau_ExposeFunctionsAsMetatable(L, collisionshape2DRegistry, "CollisionShape2DMetaTable");
    constexpr luaL_Reg area2DRegistry[] = {{"GetName", luau_Area2DGetName},
                                          {"SetName", luau_Area2DSetName},
                                          {"GetChildren", luau_Area2DGetChildren},
                                          {"GetChild", luau_Area2DGetChild},
                                          {"AddChild", luau_Area2DAddChild},
                                          {"GetPosition", luau_Area2DGetPosition},
                                          {"SetPosition", luau_Area2DSetPosition},
                                          {"GetGlobalPosition", luau_Area2DGetGlobalPosition},
                                          {"CreateChild", luau_Area2DCreateChild},
                                          {"ToggleCollision", luau_Area2DToggleCollision},
                                          {"IsCollisionEnabled", luau_Area2DIsCollisionEnabled},
                                          {"Collide", luau_Area2DCollide},
                                          {"GetSize", luau_Area2DGetSize},
                                          {"SetSize", luau_Area2DSetSize},{"Destroy", luau_Area2DDestroy},
                                          {"__gc", lua_gcArea2D},
                                          {nullptr, nullptr}};
    luau_ExposeFunctionsAsMetatable(L, area2DRegistry, "Area2DMetaTable");
    constexpr luaL_Reg parallaxRegistry[] = {{"GetName", luau_ParallaxGetName},
                                            {"SetName", luau_ParallaxSetName},
                                            {"GetChildren", luau_ParallaxGetChildren},
                                            {"GetChild", luau_ParallaxGetChild},
                                            {"AddChild", luau_ParallaxAddChild},
                                            {"CreateChild", luau_ParallaxCreateChild},
                                            {"SetReferenceNode", luau_ParallaxSetReferenceNode},
                                            {"AddParallaxPosition", luau_ParallaxAddParallaxPosition},{"Destroy", luau_ParallaxDestroy},
                                            {"__gc", lua_gcParallax},
                                            {nullptr, nullptr}};
    luau_ExposeFunctionsAsMetatable(L, parallaxRegistry, "ParallaxMetaTable");
    constexpr luaL_Reg rgbd2DRegistry[] = {{"GetName", luau_RigidBody2DGetName},
                                          {"SetName", luau_RigidBody2DSetName},
                                          {"GetChildren", luau_RigidBody2DGetChildren},
                                          {"GetChild", luau_RigidBody2DGetChild},
                                          {"AddChild", luau_RigidBody2DAddChild},
                                          {"GetPosition", luau_RigidBody2DGetPosition},
                                          {"SetPosition", luau_RigidBody2DSetPosition},
                                          {"GetGlobalPosition", luau_RigidBody2DGetGlobalPosition},
                                          {"GetVelocity", luau_RigidBody2DGetVelocity},
                                          {"SetVelocity", luau_RigidBody2DSetVelocity},
                                          {"CreateChild", luau_RigidBody2DCreateChild},
                                          {"ToggleCollision", luau_RigidBody2DToggleCollision},
                                          {"IsCollisionEnabled", luau_RigidBody2DIsCollisionEnabled},
                                          {"Collide", luau_RigidBody2DCollide},
                                          {"SetVelocity", luau_RigidBody2DSetVelocity},
                                          {"GetVelocity", luau_RigidBody2DGetVelocity},
                                          {"Destroy", luau_RigidBody2DDestroy},
                                          {"__gc", lua_gcRigidBody2D},
                                          {nullptr, nullptr}};
    luau_ExposeFunctionsAsMetatable(L, rgbd2DRegistry, "RigidBody2DMetaTable");
    constexpr luaL_Reg stbd2DRegistry[] = {{"GetName", luau_StaticBody2DGetName},
                                          {"SetName", luau_StaticBody2DSetName},
                                          {"GetChildren", luau_StaticBody2DGetChildren},
                                          {"GetChild", luau_StaticBody2DGetChild},
                                          {"AddChild", luau_StaticBody2DAddChild},
                                          {"GetPosition", luau_StaticBody2DGetPosition},
                                          {"SetPosition", luau_StaticBody2DSetPosition},
                                          {"GetGlobalPosition", luau_StaticBody2DGetGlobalPosition},
                                          {"CreateChild", luau_StaticBody2DCreateChild},
                                          {"ToggleCollision", luau_StaticBody2DToggleCollision},
                                          {"IsCollisionEnabled", luau_StaticBody2DIsCollisionEnabled},
                                          {"Collide", luau_StaticBody2DCollide},
                                          {"Destroy", luau_StaticBody2DDestroy},
                                          {"__gc", lua_gcStaticBody2D},
                                          {nullptr, nullptr}};
    luau_ExposeFunctionsAsMetatable(L, stbd2DRegistry, "StaticBody2DMetaTable");
    constexpr luaL_Reg labelRegistry[] = {{"GetName", luau_LabelGetName},
                                         {"SetName", luau_LabelSetName},
                                         {"GetChildren", luau_LabelGetChildren},
                                         {"GetChild", luau_LabelGetChild},
                                         {"AddChild", luau_LabelAddChild},
                                         {"GetPosition", luau_LabelGetPosition},
                                         {"SetPosition", luau_LabelSetPosition},
                                         {"GetGlobalPosition", luau_LabelGetGlobalPosition},
                                         {"CreateChild", luau_LabelCreateChild},
                                         {"SetColor", luau_LabelSetColor},
                                         {"SetAlpha", luau_LabelSetAlpha},
                                         {"SetFont", luau_LabelSetFont},
                                         {"GetFont", luau_LabelGetFont},
                                         {"SetText", luau_LabelSetText},
                                         {"GetText", luau_LabelGetText},
                                         {"SetFontSize", luau_LabelSetFontSize},
                                         {"GetFontSize", luau_LabelGetFontSize},
                                         {"Destroy", luau_LabelDestroy},
                                         {"__gc", lua_gcLabel},
                                         {nullptr, nullptr}};
    luau_ExposeFunctionsAsMetatable(L, labelRegistry, "LabelMetaTable");
    constexpr luaL_Reg boxRegistry[] = {{"GetName", luau_BoxGetName},
                                       {"SetName", luau_BoxSetName},
                                       {"GetChildren", luau_BoxGetChildren},
                                       {"GetChild", luau_BoxGetChild},
                                       {"AddChild", luau_BoxAddChild},
                                       {"GetPosition", luau_BoxGetPosition},
                                       {"SetPosition", luau_BoxSetPosition},
                                       {"GetGlobalPosition", luau_BoxGetGlobalPosition},
                                       {"CreateChild", luau_BoxCreateChild},
                                       {"SetColor", luau_BoxSetColor},
                                       {"GetColor", luau_BoxGetColor},
                                       {"SetAlpha", luau_BoxSetAlpha},
                                       {"GetAlpha", luau_BoxGetAlpha},
                                       {"SetSize", luau_BoxSetSize},
                                       {"GetSize", luau_BoxGetSize},
                                       {"Destroy", luau_BoxDestroy},
                                       {"__gc", lua_gcBox},
                                       {nullptr, nullptr}};
    luau_ExposeFunctionsAsMetatable(L, boxRegistry, "BoxMetaTable");
    constexpr luaL_Reg soundPlayerLibrary[] = {{"GetName", luau_SoundPlayerGetName},
                                        {"SetName", luau_SoundPlayerSetName},
                                        {"GetChildren", luau_SoundPlayerGetChildren},
                                        {"GetChild", luau_SoundPlayerGetChild},
                                        {"AddChild", luau_SoundPlayerAddChild},
                                        {"CreateChild", luau_SoundPlayerCreateChild},
                                        {"Destroy", luau_SoundPlayerDestroy},
                                        {"Play", luau_SoundPlayerPlay},
                                        {"Stop", luau_SoundPlayerStop},
                                        {"Pause", luau_SoundPlayerPause},
                                        {"Resume", luau_SoundPlayerResume},
                                        {"SetVolume", luau_SoundPlayerSetVolume},
                                        {"SetPitch", luau_SoundPlayerSetPitch},
                                        {"SetPan", luau_SoundPlayerSetPan},
                                        {"SetSound", luau_SoundPlayerSetSound},
                                        {"GetSound", luau_SoundPlayerGetSound},
                                        {"IsPlaying", luau_SoundPlayerIsPlaying},
                                        {"IsStopped", luau_SoundPlayerIsStopped},
                                        {"__gc", lua_gcSoundPlayer},
                                        {nullptr, nullptr}};
    luau_ExposeFunctionsAsMetatable(L, soundPlayerLibrary, "SoundPlayerMetaTable");
    /* NODE LIBRARY */

    /* ENGINE LIBRARY */
    constexpr luaL_Reg engineLibrary[] = {
        {"CreateNode", luau_EngineCreateNode},
        {nullptr, nullptr}};
    luau_ExposeFunctionsAsLibrary(L, engineLibrary, "engine");
    /* ENGINE LIBRARY */
}

/* LUA API LIBRARY */
