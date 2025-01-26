include("common/game.luau")
include("common/brick.luau")
include("common/ball.luau")

function defaultPaddle(id: number)
    return {
        id = id,
        bound_player = nil,
        node = nil,
        actions = {
            MOVE_LEFT = false,
            MOVE_RIGHT = false,
            MOVE_UP = false,
            MOVE_DOWN = false,
            SHOOT = false
        },
        buffered_shot = false,
        shoot_cd = 0.0,
        soaps = 3
    }
end

function getPaddleByPlayer(ply: string)
    for i = 1, #paddles do
        if paddles[i].bound_player == ply then
            return paddles[i]
        end
    end
end

function spawnPaddle(id: number, ply: string)
    paddles[id].bound_player = ply
    paddles[id].node = root:CreateChild("RigidBody2D", "Paddle" .. id, 300, 565, Game.playersSize.x, Game.playersSize.y, 0, 0)
end

function despawnPaddle(id: number)
    if paddles[id].node ~= nil then
        paddles[id].node:Destroy()
    end
    paddles[id].node = nil
    paddles[id].shoot_cd = 0.0
    paddles[id].respawn_cd = RESPAWN_TIME
    table.foreach(paddles[id].actions, function(k, v)
        paddles[id].actions[k] = false
    end)
end

function updatePaddleVelocity(paddle, dt)
    local vx, vy = paddle.node:GetVelocity()
    if paddle.actions.MOVE_LEFT then
        paddle.node:SetVelocity(-Game.playerSpeed, vy)
    end
    if paddle.actions.MOVE_RIGHT then
        paddle.node:SetVelocity(Game.playerSpeed, vy)
    end
    if not paddle.actions.MOVE_LEFT and not paddle.actions.MOVE_RIGHT then
        paddle.node:SetVelocity(0.0, vy)
    end
end

function syncNewPlayer(ply: string)
    for i = 1, #paddles do
        if paddles[i].bound_player == nil then continue end
        if paddles[i].bound_player == ply then continue end
        net.Start("PaddleSpawn")
            net.WriteInt(i)
        net.SendToClient(ply)
    end
    for k, v in pairs(bricks) do
        local x, y = v:GetPosition()
        local is_yucky = #v:GetChildren() > 1
        net.Start("BrickSpawn")
            net.WriteString(k)
            net.WriteFloat(x)
            net.WriteFloat(y)
            net.WriteBool(is_yucky)
        net.SendToClient(ply)
    end
end

function handleShooting(ply, delta)
    if ply.buffered_shot then
        if ply.shoot_cd <= 0.0 then
            ply.shoot_cd = Game.shootCooldown
            ply.buffered_shot = false
            shoot(ply)
        end
    end
    if ply.shoot_cd > 0.0 then
        ply.shoot_cd = ply.shoot_cd - delta
    end
end

function shoot(ply)
    if ply.soaps <= 0 then return end
    ply.soaps = ply.soaps - 1
    local x, y = ply.node:GetPosition()
    x = x + Game.playersSize.x / 2
    y = y - Game.playersSize.y / 2
    local id = generateUUID()
    local ball = generateBall(x, y, id)
    local ply_x, ply_y = ply.node:GetVelocity()
    ball:SetVelocity(ply_x / 2, -Game.ballSpeed)
end

function broadcastBallsPosition()
    for k, v in pairs(balls) do
        local x, y = v:GetPosition()
        net.Start("BallMovement")
            net.WriteString(k)
            net.WriteFloat(x)
            net.WriteFloat(y)
        net.Broadcast()
    end
end
