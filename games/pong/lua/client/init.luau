include("client/utils.luau")
include("client/ball.luau")
include("client/wall.luau")
include("client/player.luau")

local player = generatePlayer(5, GetWindowHeight() / 2 - 50, 1)
local player2 = generatePlayer(GetWindowWidth() - 5 - ENTITY_WIDTH, GetWindowHeight() / 2 - 50, 2)
local ball = generateBall()
generateMiddle(14, 10)

hook.Add("Tick", "main", function(delta)
    local dt = delta / 1000000000
    playerMovement(player, dt, keymap.key_s, keymap.key_w) -- D, A
    playerMovement(player2, dt, keymap.key_k, keymap.key_i) -- I, K
    ballWallBounce(ball)
    ballPlayerBounce(ball, player)
    ballPlayerBounce(ball, player2)
end)