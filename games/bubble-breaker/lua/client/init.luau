include("client/player.luau")
include("client/utils.luau")

local player = generatePlayer(WINDOW_WIDTH / 2 - 50, 565, 1)

hook.Add("Tick", "main", function(delta)
    local dt = delta / 1000000000
    playerMovement(player, dt, keymap.key_a, keymap.key_d)
end)