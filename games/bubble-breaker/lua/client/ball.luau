include("client/utils.luau")

local function generateBallVelocity()
    local angle = math.rad(math.random(30, 150))

    local x_vel = math.cos(angle) * BALL_VELOCITY_COEFFICIENT
    local y_vel = math.sin(angle) * BALL_VELOCITY_COEFFICIENT

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
    ball:SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2)
    local x_vel, y_vel = generateBallVelocity()
    local ball_body = ball:CreateChild("RigidBody2D", "ball_body", 0, 0, ENTITY_WIDTH, ENTITY_WIDTH, x_vel, y_vel)
    local ball_sprite = ball_body:CreateChild("Sprite2D", "ball_sprite", "assets/ball.png") --change this to the correct path


    ball_sprite:SetSize(ENTITY_WIDTH, ENTITY_WIDTH)
    return ball
end