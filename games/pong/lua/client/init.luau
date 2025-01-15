local player = root:CreateChild("Node2D", "player")
local player_sprite = player:CreateChild("Sprite2D", "player_sprite", "assets/player.png")

player:SetPosition(5, 600 / 2 - 40)
player_sprite:SetSize(14, 100)

function localPlayerMovement(dt)
    local x, y = player:GetPosition()
    local speed = 240

    if IsKeyPressed(65) then
        y -= speed * dt
    end
    if IsKeyPressed(68) then
        y += speed * dt
    end
    player:SetPosition(x, y)
end

hook.Add("Tick", "main", function(delta)
    local dt = delta / 1000000000
    localPlayerMovement(dt)
end)