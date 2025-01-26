include("client/player.luau")
include("common/game.luau")
include("client/brick.luau")
include("client/ball.luau")

localPlayer = {
    id = -1,
    node = nil
}
otherPlayer = {
    id = -1,
    node = nil
}

local brick = generateBrick(100, 100, 1)
local ball = generateBall()

hook.Add("Tick", "main", function(delta)
    local dt = delta / 1000000000
    if localPlayer.id ~= -1 then
        playerMovement(localPlayer.node, dt, keymap.key_left, keymap.key_right)
    end
end)

net.Receive("PaddleSpawn", function(len, ply)
    local nb = net.ReadInt()
    if localPlayer.id == nb or otherPlayer.id == nb then return end
    local paddle = generatePlayer(Game.windowSize.x / 2 - 64, 565, nb)
    if localPlayer.id == -1 then
        localPlayer.id = nb
        localPlayer.node = paddle
    else
        otherPlayer.id = nb
        otherPlayer.node = paddle
    end
end)

net.Receive("PaddleDespawn", function(len, ply)
    local nb = net.ReadInt()
    if nb == localPlayer.id then
        deletePlayer(localPlayer.node)
        localPlayer.id = -1
        localPlayer.node = nil
    else
        deletePlayer(otherPlayer.node)
        otherPlayer.id = -1
        otherPlayer.node = nil
    end
end)

net.Receive("PaddleMovement", function(len, ply)
    local nb = net.ReadInt()
    local x = net.ReadFloat()
    local y = net.ReadFloat()
    if nb == localPlayer.id then
        correctPosition(localPlayer.node, x, y)
    else
        correctPosition(otherPlayer.node, x, y)
    end
end)
