-- Generates the player
-- Arguments: None
-- Returns: Node2D

function generatePlayer(x_pos, y_pos, player_nb)
    local player_name = "player_" .. player_nb
    local player = root:CreateChild("Node2D", player_name)
    player:SetPosition(x_pos, y_pos)
    local player_sprite = player:CreateChild("Sprite2D", "player_sprite", "assets/player.png")
    player_sprite:SetSize(ENTITY_WIDTH, 100)
    player:CreateChild("Area2D", "player_hitbox", 0, 0, ENTITY_WIDTH, 100)
    return player
end

-- Movement for the local player
-- Arguments: player (node), dt (number)
-- Returns: None
function playerMovement(player, dt, upKey, downKey)
    local x, y = player:GetPosition()
    local speed = 240

    if IsKeyPressed(downKey) then
        y = y - speed * dt
    end
    if IsKeyPressed(upKey) then
        y = y + speed * dt
    end
    player:SetPosition(x, y)
end
