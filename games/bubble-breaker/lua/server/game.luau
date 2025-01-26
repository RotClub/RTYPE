include("common/game.luau")
include("common/brick.luau")

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
        shoot_cd = 0.0
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
        net.Start("BrickSpawn")
            net.WriteInt(k)
            net.WriteFloat(x)
            net.WriteFloat(y)
        net.SendToClient(ply)
    end
end
