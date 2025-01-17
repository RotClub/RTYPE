include("client/utils.luau")
include("client/ball.luau")
include("client/wall.luau")
include("client/player.luau")
include("client/score.luau")

local player = generatePlayer(5, 600 / 2 - 50, 1) --GetWindowHeight() / 2 - 50, 1)
local player2 = generatePlayer(800 - 5 - ENTITY_WIDTH, 600 / 2 - 50, 2)--GetWindowWidth() - 5 - ENTITY_WIDTH, GetWindowHeight() / 2 - 50, 2)
local ball = generateBall()
generateMiddle(14, 10)
local score = generateScore()
score_text = drawScore(score, 1)
score_text_player2 = drawScore(score, 2)

hook.Add("Tick", "main", function(delta)
    local dt = delta / 1000000000
    playerMovement(player, dt, keymap.key_s, keymap.key_w) -- D, A
    playerMovement(player2, dt, keymap.key_k, keymap.key_i) -- I, K
    ballWallBounce(ball)
    ballPlayerBounce(ball, player)
    ballPlayerBounce(ball, player2)
    score = ballPlayerGoal(ball, score)
    updateScore(score, 1, score_text)
    updateScore(score, 2, score_text_player2)
end)