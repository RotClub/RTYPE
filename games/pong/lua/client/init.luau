include("client/utils.luau")
include("client/ball.luau")
include("client/wall.luau")
include("client/player.luau")
include("client/score.luau")

local player = generatePlayer()
local ball = generateBall()
generateMiddle(14, 10)
local score = generateScore()
score_text = drawScore(score, 1)
score_text_player2 = drawScore(score, 2)

hook.Add("Tick", "main", function(delta)
    local dt = delta / 1000000000
    playerMovement(player, dt)
    ballWallBounce(ball)
    ballPlayerBounce(ball, player)
end)