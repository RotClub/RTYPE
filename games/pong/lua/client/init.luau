include("client/utils.luau")
include("client/ball.luau")
include("client/wall.luau")

local player = root:CreateChild("Node2D", "player")
player:SetPosition(5, 600 / 2 - 50)
local player_sprite = player:CreateChild("Sprite2D", "player_sprite", "assets/player.png")
player_sprite:SetSize(14, 100)
local player_hitbox = player:CreateChild("Area2D", "hitbox", 0, 0, 14, 100)

local player2 = engine:CreateNode("Node2D", "player2")
player2:SetPosition(5, 600 / 2 - 50 + 150)
local player_sprite2 = player2:CreateChild("Sprite2D", "player_sprite2", "assets/player.png")
player_sprite2:SetSize(14, 100)
local player_hitbox2 = player2:CreateChild("Area2D", "hitbox2", 0, 0, 14, 100)

root:AddChild(player2)

local ball = generateBall()
generateMiddle(14, 10)

function localPlayerMovement(dt)
    local x, y = player:GetPosition()
    local speed = 240

    if player_hitbox:Collide(player_hitbox2) then
        info("Collided")
    else
        info("Not Collided")
    end

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