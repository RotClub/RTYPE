include("client/player.luau")
include("common/game.luau")
include("client/brick.luau")
include("client/ball.luau")

Game.defaultReferencePoint = root:CreateChild("Node2D", "defaultReferencePoint")
Game.defaultReferencePoint:SetPosition(0, 0)

local parallaxLayers = {
    root:CreateChild("Parallax", "parallaxBg1", "assets/clouds/1.png", 1, Game.defaultReferencePoint),
    root:CreateChild("Parallax", "parallaxBg2", "assets/clouds/2.png", 1.5, Game.defaultReferencePoint),
    root:CreateChild("Parallax", "parallaxBg3", "assets/clouds/3.png", 2, Game.defaultReferencePoint),
    root:CreateChild("Parallax", "parallaxBg4", "assets/clouds/4.png", 2.5, Game.defaultReferencePoint),
    root:CreateChild("Parallax", "parallaxBg5", "assets/clouds/5.png", 3, Game.defaultReferencePoint)
}

Game.localId = nil

function setParallaxReference(ref)
    for i = 1, #parallaxLayers do
        parallaxLayers[i]:SetReferenceNode(ref)
        debug("Set parallax reference to " .. ref:GetName())
    end
end

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
    if localPlayer.node then
        playerMovement(localPlayer.node, dt, 65, 68)
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
