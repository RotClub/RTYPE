-- Generates the player
-- Arguments: None
-- Returns: Node2D
function generatePlayer()
    local player = root:CreateChild("Node2D", "player")
    player:SetPosition(5, GetWindowHeight() / 2 - 50)
    local player_sprite = player:CreateChild("Sprite2D", "player_sprite", "assets/player.png")
    player_sprite:SetSize(14, 100)
    player:CreateChild("Area2D", "player_hitbox", 0, 0, 14, 100)

    return player
end

-- Movement for the local player
-- Arguments: player (node), dt (number)
-- Returns: None
function playerMovement(player, dt)
    local x, y = player:GetPosition()
    local speed = 240

    if IsKeyPressed(65) then
        y = y - speed * dt
    end
    if IsKeyPressed(68) then
        y = y + speed * dt
    end
    player:SetPosition(x, y)
end
