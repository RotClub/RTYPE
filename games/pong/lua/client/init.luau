local player = root:CreateChild("Node2D", "player")
local player_sprite = player:CreateChild("Sprite2D", "player_sprite", "assets/player.png")

player:SetPosition(0, 0)
player_sprite:SetSize(32, 32)

function localPlayerMovement(dt)
    local x, y = localPlayer:GetPosition()

    if IsKeyPressed(65) then
        x -= 70 * dt
    end
    if IsKeyPressed(68) then
        x += 70 * dt
    end
    player:SetPosition(x, y)
end