include("common/game.luau")

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
    paddles[id].node = root:CreateChild("RigidBody2D", "Paddle" .. id, 400, 600, Game.playersSize.x, Game.playersSize.y, 0, 0)
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
