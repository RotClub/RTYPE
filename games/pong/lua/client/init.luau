include("client/utils.luau")
include("client/ball.luau")
include("client/wall.luau")
include("client/player.luau")

local player = generatePlayer()
local ball = generateBall()
generateMiddle(14, 10)

hook.Add("Tick", "main", function(delta)
    local dt = delta / 1000000000
    playerMovement(player, dt)
    ballWallBounce(ball)
    ballPlayerBounce(ball, player)
end)