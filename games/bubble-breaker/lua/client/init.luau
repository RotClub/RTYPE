include("client/player.luau")
include("common/game.luau")
include("common/brick.luau")
include("common/ball.luau")

Game.defaultReferencePoint = root:CreateChild("Node2D", "defaultReferencePoint")
Game.defaultReferencePoint:SetPosition(0, 0)

local parallaxLayers = {
    root:CreateChild("Parallax", "parallaxBg1", "assets/clouds/1v2.png", 1, Game.defaultReferencePoint),
    root:CreateChild("Parallax", "parallaxBg2", "assets/clouds/2v2.png", 1.5, Game.defaultReferencePoint),
    root:CreateChild("Parallax", "parallaxBg3", "assets/clouds/3v2.png", 2, Game.defaultReferencePoint),
    root:CreateChild("Parallax", "parallaxBg4", "assets/clouds/4v2.png", 2.5, Game.defaultReferencePoint),
    root:CreateChild("Parallax", "parallaxBg5", "assets/clouds/5v2.png", 3, Game.defaultReferencePoint)
}

Game.localId = nil

localPlayer = {
    id = -1,
    node = nil
}
otherPlayer = {
    id = -1,
    node = nil
}

hook.Add("Tick", "main", function(delta)
    local dt = delta / 1000000000
    if localPlayer.node then
        playerMovement(localPlayer.node, dt, 65, 68)
        for k, balls in pairs(balls) do
            balls:GetChild("ball_sprite"):SetRotation(balls:GetChild("ball_sprite"):GetRotation() + 0.1)
        end
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

net.Receive("BrickSpawn", function(len, ply)
    local brick_nb = net.ReadString()
    local x_pos = net.ReadFloat()
    local y_pos = net.ReadFloat()
    bricks[brick_nb] = generateBrick(x_pos, y_pos, brick_nb)
end)

net.Receive("BallSpawn", function(len, ply)
    local id = net.ReadString()
    local x_pos = net.ReadFloat()
    local y_pos = net.ReadFloat()
    generateBall(x_pos, y_pos, id)
end)
