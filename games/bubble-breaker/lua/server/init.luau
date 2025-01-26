include("server/game.luau")
include("server/network.luau")
include("common/ball.luau")

paddles = {
    defaultPaddle(1),
    defaultPaddle(2)
}

brickLine(16, 0, 11)
brickLine(16, 32, 11)
brickLine(16, 64, 11)
brickLine(16, 96, 11)
brickLine(16, 128, 11)
brickLine(16, 160, 11)

hook.Add("Tick", "main", function(delta)
    delta = delta / 1_000_000_000
    for i = 1, #paddles do
        if not paddles[i].bound_player or not paddles[i].node then continue end
        updatePaddleVelocity(paddles[i], delta)
        sendPaddlePosition(paddles[i])
        handleShooting(paddles[i], delta)
    end
    for k, ball in pairs(balls) do
        ballWallBounce(ball, k)
        for i = 1, #paddles do
            if paddles[i].bound_player then
                ballPlayerBounce(ball, paddles[i].node)
            end
        end
    end
    broadcastBallsPosition()
end)

hook.Add("PlayerJoin", "main", function(ply)
    for i = 1, #paddles do
        if paddles[i].bound_player == nil then
            spawnPaddle(i, ply)
            net.Start("PaddleSpawn")
                net.WriteInt(i)
            net.SendToClient(ply)
            net.Start("PaddleSpawn")
                net.WriteInt(i)
            net.Broadcast()
            break
        end
    end
    syncNewPlayer(ply)
end)

hook.Add("PlayerLeave", "main", function(ply)
    for i = 1, #paddles do
        if paddles[i].bound_player == ply then
            paddles[i].bound_player = nil
            if paddles[i].node then
                paddles[i].node:Destroy()
                paddles[i].node = nil
            end
            table.foreach(paddles[i].actions, function(k, v)
                paddles[i].actions[k] = false
            end)
            net.Start("PaddleDespawn")
                net.WriteInt(i)
                net.WriteBool(false)
            net.Broadcast()
        end
    end
end)
