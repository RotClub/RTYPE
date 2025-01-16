include("client/utils.luau")

-- Desc: Ball generation function
-- Arguments: None
-- Returns: Node2D
function generateBall()
    local ball = root:CreateChild("Node2D", "ball")
    local x_vel = math.random() * (math.random(-1, 1) < 0 and -1 or 1) * BALL_VELOCITY_COEFFICIENT
    local y_vel = math.random() * (math.random(-1, 1) < 0 and -1 or 1) * BALL_VELOCITY_COEFFICIENT
    local ball_body = ball:CreateChild("RigidBody2D", "ball_body", 0, 0, x_vel, y_vel)
    local ball_sprite = ball_body:CreateChild("Sprite2D", "ball_sprite", "assets/player.png")

    ball:SetPosition(GetWindowWidth() / 2, GetWindowHeight() / 2)
    --ball:SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2)
    ball_sprite:SetSize(14, 14)
    return ball
end

function ballBounce(ball)
    local rgbd = ball:GetChild("ball_body")
    local x_vel, y_vel = rgbd:GetVelocity()
    local x_pos, y_pos = rgbd:GetGlobalPosition()

    local max_y = GetWindowHeight --engine:GetRenderHeight()
    local min_y = 0

    -- Debugging: Print current position and velocity
    info("Ball Position: x = " .. x_pos .. ", y = " .. y_pos)
    info("Ball Velocity: x_vel = " .. x_vel .. ", y_vel = " .. y_vel)

    -- Bounce the ball off the top and bottom walls
    if y_pos < min_y then
        info("\n\n\n\nBall hit the top wall!\n\n\n\n")
        y_vel = math.abs(y_vel) -- Reflect upwards
    elseif y_pos > max_y then
        info("\n\n\n\nBall hit the bottom wall!\n\n\n\n")
        y_vel = -math.abs(y_vel) -- Reflect downwards
    end

    -- Update the ball's velocity
    rgbd:SetVelocity(x_vel, y_vel)

    -- Debugging: Print updated velocity
    info("Updated Velocity: x_vel = " .. x_vel .. ", y_vel = " .. y_vel)
end
