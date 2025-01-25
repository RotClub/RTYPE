include("common/game.luau")

local function generateBallVelocity()
    local angle = math.rad(math.random(30, 150))

    local x_vel = math.cos(angle) * Game.ballVelocityCoefficient
    local y_vel = math.sin(angle) * Game.ballVelocityCoefficient

    if math.random(0, 1) == 0 then
        x_vel = -x_vel
    end
    if math.random(0, 1) == 0 then
        y_vel = -y_vel
    end

    return x_vel, y_vel
end

function generateBall()
    local ball = root:CreateChild("Node2D", "ball")
    ball:SetPosition(Game.windowSize.x / 2, Game.windowSize.y / 2)
    local x_vel, y_vel = generateBallVelocity()
    local ball_body = ball:CreateChild("RigidBody2D", "ball_body", 0, 0, Game.ballSize, Game.ballSize, x_vel, y_vel)
    local ball_sprite = ball_body:CreateChild("Sprite2D", "ball_sprite", "assets/bricks/Colored_Purple-64x32.png") --change this to the correct path
    ball_sprite:SetSize(Game.ballSize, Game.ballSize)
    return ball
end

function ballWallBounce(ball)
    local rgbd = ball:GetChild("ball_body")
    local x_vel, y_vel = rgbd:GetVelocity()
    local x_pos, y_pos = rgbd:GetGlobalPosition()

    local max_y = Game.windowSize.y
    local min_y = 0
    local max_x = Game.windowSize.x
    local min_x = 0

    if x_pos < min_x or x_pos > max_x then
        x_vel = -x_vel
    end
    if y_pos < min_y or y_pos > max_y then
        y_vel = -y_vel
    end

    rgbd:SetVelocity(x_vel, y_vel)
end

function ballPlayerBounce(ball, player)
    local ball_rgbd = ball:GetChild("ball_body")
    local player_hitbox = player:GetChild("player_hitbox")

    if ball_rgbd:Collide(player_hitbox) then
        local x_vel, y_vel = ball_rgbd:GetVelocity()
        local _, y_pos = ball_rgbd:GetGlobalPosition()
        local _, player_y = player:GetPosition()
        local _, player_height = player_hitbox:GetSize()

        local player_center = player_y + player_height / 2
        local ball_center = y_pos + Game.ballSize / 2

        local y_diff = ball_center - player_center
        local y_refract = y_diff / (player_height / 2)

        ball_rgbd:SetVelocity(-x_vel, y_vel + y_refract)
    end
end
