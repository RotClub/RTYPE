include("client/utils.luau")
include("client/ball.luau")
include("client/wall.luau")
include("client/player.luau")
include("client/score.luau")

local player = generatePlayer()
local ball = generateBall()
generateMiddle(14, 10)
local score = generateScore()
drawScore(score)

hook.Add("Tick", "main", function(delta)
    local dt = delta / 1000000000
    playerMovement(player, dt)
    ballWallBounce(ball)
    ballPlayerBounce(ball, player)
end)