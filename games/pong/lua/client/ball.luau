include("client/utils.luau")

local function generateBallVelocity()
    -- Generate a random angle between 30 and 150 degrees (or the equivalent in radians)
    local angle = math.rad(math.random(30, 150))

    -- Calculate velocity components based on the angle
    local x_vel = math.cos(angle) * BALL_VELOCITY_COEFFICIENT
    local y_vel = math.sin(angle) * BALL_VELOCITY_COEFFICIENT

    -- Randomly invert the direction
    if math.random(0, 1) == 0 then
        x_vel = -x_vel
    end
    if math.random(0, 1) == 0 then
        y_vel = -y_vel
    end

    return x_vel, y_vel
end

-- Desc: Ball generation function
-- Arguments: None
-- Returns: Node2D
function generateBall()
    local ball = root:CreateChild("Node2D", "ball")
    local x_vel, y_vel = generateBallVelocity()
    local ball_body = ball:CreateChild("RigidBody2D", "ball_body", 0, 0, ENTITY_WIDTH, ENTITY_WIDTH, x_vel, y_vel)
    local ball_sprite = ball_body:CreateChild("Sprite2D", "ball_sprite", "assets/player.png")

    ball:SetPosition(800 / 2, 600 / 2)--GetWindowWidth() / 2, GetWindowHeight() / 2)
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

    local max_y = 600 --GetWindowHeight()
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

function ballPlayerGoal(ball, score)
    local ball_rgbd = ball:GetChild("ball_body")
    local x_pos, _ = ball_rgbd:GetGlobalPosition()
    local max_x = 800 --GetWindowWidth()
    local min_x = 0

    if x_pos <= min_x then
        ball:SetPosition(800 / 2, 600 / 2)--GetWindowWidth() / 2, GetWindowHeight() / 2)
        ball_rgbd:SetVelocity(generateBallVelocity())
        score[2] = score[2] + 1
    elseif x_pos >= max_x then
        ball:SetPosition(800 / 2, 600 / 2)--GetWindowWidth() / 2, GetWindowHeight() / 2)
        ball_rgbd:SetVelocity(generateBallVelocity())
        score[1] = score[1] + 1
    end

    return score
end
