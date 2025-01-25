include("common/game.luau")

function generatePlayer(x_pos, y_pos, player_nb)
    local player_name = "player_" .. player_nb
    local player = root:CreateChild("Node2D", player_name)
    player:SetPosition(x_pos, y_pos)
    if player_nb == 1 then
        local player_sprite = player:CreateChild("Sprite2D", "player_sprite", "assets/paddle/Paddle_B_Purple_128x28.png")
        player_sprite:SetSize(Game.playersSize.x, Game.playersSize.y)
    else
        local player_sprite = player:CreateChild("Sprite2D", "player_sprite", "assets/paddle/Paddle_B_Yellow_128x28.png")
        player_sprite:SetSize(Game.playersSize.x, Game.playersSize.y)
    end
    player:CreateChild("Area2D", "player_hitbox", 0, 0, Game.playersSize.x, Game.playersSize.y)
    return player
end

function playerMovement(player, dt, leftKey, rightKey)
    local x, y = player:GetPosition()
    local speed = 600

    if IsKeyPressed(leftKey) then
        x = x - speed * dt
    end
    if IsKeyPressed(rightKey) then
        x = x + speed * dt
    end
    if IsKeyJustPressed(leftKey) then
        sendAction(1, true)
    end
    if IsKeyJustPressed(rightKey) then
        sendAction(2, true)
    end
    if IsKeyJustReleased(leftKey) then
        sendAction(1, false)
    end
    if IsKeyJustReleased(rightKey) then
        sendAction(2, false)
    end
    player:SetPosition(x, y)
end