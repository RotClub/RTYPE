include("client/utils.luau")
include("client/ball.luau")
include("client/wall.luau")
include("client/player.luau")
include("client/score.luau")

local player = generatePlayer(5, WINDOW_HEIGHT / 2 - 50, 1)
local player2 = generatePlayer(WINDOW_WIDTH - 5 - ENTITY_WIDTH, WINDOW_HEIGHT / 2 - 50, 2)
local ball = generateBall()
generateMiddle(14, 10)
local score = generateScore()
local score_text = drawScore(score, 1)
local score_text_player2 = drawScore(score, 2)

hook.Add("Tick", "main", function(delta)
    local dt = delta / 1000000000
    playerMovement(player, dt, keymap.key_s, keymap.key_w) -- D, A
    playerMovement(player2, dt, keymap.key_k, keymap.key_i) -- I, K
    ballWallBounce(ball)
    ballPlayerBounce(ball, player)
    ballPlayerBounce(ball, player2)
    score = ballPlayerGoal(ball, score)
    updateScore(score, score_text, score_text_player2)
end)