balls = {}

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

function generateBall(x: number, y: number, id: string)
    local x_vel, y_vel = 0, 0
    if SERVER then
        x_vel, y_vel = generateBallVelocity()
    end
    local ball = root:CreateChild("RigidBody2D", "ball-" .. id, x, y, Game.ballSize, Game.ballSize, x_vel, y_vel)
    if CLIENT then
        local ball_sprite = ball:CreateChild("Sprite2D", "ball_sprite", "assets/soap.png")
        ball_sprite:SetSize(Game.ballSize, Game.ballSize)
        ball_sprite:SetOrigin(Game.ballSize / 2, Game.ballSize / 2)
    elseif SERVER then
        net.Start("BallSpawn")
            net.WriteString(id)
            net.WriteFloat(x)
            net.WriteFloat(y)
        net.Broadcast()
    end
    balls[id] = ball
end

function ballWallBounce(ball)
    local x_vel, y_vel = ball:GetVelocity()
    local x_pos, y_pos = ball:GetGlobalPosition()

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

    ball:SetVelocity(x_vel, y_vel)
end

function ballPlayerBounce(ball, player)
    local player_hitbox = player:GetChild("player_hitbox")

    if ball:Collide(player_hitbox) then
        local x_vel, y_vel = ball:GetVelocity()
        local _, y_pos = ball:GetGlobalPosition()
        local _, player_y = player:GetPosition()
        local _, player_height = player_hitbox:GetSize()

        local player_center = player_y + player_height / 2
        local ball_center = y_pos + Game.ballSize / 2

        local y_diff = ball_center - player_center
        local y_refract = y_diff / (player_height / 2)

        ball:SetVelocity(-x_vel, y_vel + y_refract)
    end
end
