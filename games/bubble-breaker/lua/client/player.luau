include("common/game.luau")

function generatePlayer(x_pos, y_pos, player_nb)
    local player_name = "player_" .. player_nb
    local player = root:CreateChild("RigidBody2D", "Paddle" .. player_nb, x_pos, y_pos, Game.playersSize.x, Game.playersSize.y, 0, 0)
    if player_nb == 1 then
        local player_sprite = player:CreateChild("Sprite2D", "player_sprite", "assets/paddle/Paddle_B_Purple_128x28.png")
        player_sprite:SetSize(Game.playersSize.x, Game.playersSize.y)
    else
        local player_sprite = player:CreateChild("Sprite2D", "player_sprite", "assets/paddle/Paddle_B_Yellow_128x28.png")
        player_sprite:SetSize(Game.playersSize.x, Game.playersSize.y)
    end
    return player
end

function deletePlayer(player)
    player:Destroy()
end

function sendAction(action, state)
    if state then
        net.Start("ActionStart")
            net.WriteInt(action)
        net.SendToServer()
    else
        net.Start("ActionEnd")
            net.WriteInt(action)
        net.SendToServer()
    end
end

function playerMovement(player, dt, leftKey, rightKey)
    local vx, yx = player:GetVelocity()
    if IsKeyPressed(leftKey) then
        player:SetVelocity(Game.playerSpeed * -1.0, yx)
    end
    if IsKeyPressed(rightKey) then
        player:SetVelocity(Game.playerSpeed, yx)
    end
    if not IsKeyPressed(leftKey) and not IsKeyPressed(rightKey) then
        player:SetVelocity(0.0, yx)
    end

    if IsKeyJustPressed(leftKey) then
        sendAction(1, true)
    end
    if IsKeyJustPressed(rightKey) then
        sendAction(2, true)
    end
    if IsKeyReleased(leftKey) then
        sendAction(1, false)
    end
    if IsKeyReleased(rightKey) then
        sendAction(2, false)
    end
    if IsKeyJustPressed(74) then
        sendAction(5, true)
        sendAction(5, false)
    end
end

function correctPosition(entity, x: number, y: number)
    local eX, eY = entity:GetGlobalPosition()
    entity:SetPosition((eX + x) / 2, (eY + y) / 2)
end
