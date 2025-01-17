include("client/utils.luau")

-- Desc: Ball generation function
-- Arguments: None
-- Returns: Node2D
function generateBall()
    local ball = root:CreateChild("Node2D", "ball")
    local x_vel = math.random(-1, 1) * BALL_VELOCITY_COEFFICIENT
    local y_vel = math.random(-1, 1) * BALL_VELOCITY_COEFFICIENT
    local ball_body = ball:CreateChild("RigidBody2D", "ball_body", 0, 0, ENTITY_WIDTH, ENTITY_WIDTH, x_vel, y_vel)
    local ball_sprite = ball_body:CreateChild("Sprite2D", "ball_sprite", "assets/player.png")

    ball:SetPosition(GetWindowWidth() / 2, GetWindowHeight() / 2)
    ball_sprite:SetSize(ENTITY_WIDTH, ENTITY_WIDTH)
    return ball
end

-- Desc: Ball wall bounce function
-- Arguments: Node2D
-- Returns: None
function ballWallBounce(ball)
    local rgbd = ball:GetChild("ball_body")
    local x_vel, y_vel = rgbd:GetVelocity()
    local _, y_pos = rgbd:GetGlobalPosition()

    local max_y = GetWindowHeight()
    local min_y = 0

    if y_pos < min_y then
        y_vel = math.abs(y_vel) -- Reflect upwards
    elseif y_pos > max_y then
        y_vel = -math.abs(y_vel) -- Reflect downwards
    end

    -- Update the ball's velocity
    rgbd:SetVelocity(x_vel, y_vel)
end

-- Compute the ball refraction on player hitbox
-- Arguments: ball (node), player (node)
-- Returns: None
function ballPlayerBounce(ball, player)
    local ball_rgbd = ball:GetChild("ball_body")
    local player_hitbox = player:GetChild("player_hitbox")

    if ball_rgbd:Collide(player_hitbox) then
        info("colliding")
        local x_vel, y_vel = ball_rgbd:GetVelocity()
        local _, y_pos = ball_rgbd:GetGlobalPosition()
        local _, player_y = player:GetPosition()
        local _, player_height = player_hitbox:GetSize()

        local player_center = player_y + player_height / 2
        local ball_center = y_pos + ENTITY_WIDTH / 2

        local y_diff = ball_center - player_center
        local y_refract = y_diff / (player_height / 2)

        ball_rgbd:SetVelocity(-x_vel, y_vel + y_refract)
    end
end
