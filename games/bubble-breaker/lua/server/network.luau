local ACTIONS = {
    "MOVE_LEFT",
    "MOVE_RIGHT",
    "MOVE_UP",
    "MOVE_DOWN",
    "SHOOT"
}

local function handleActionChange(ply: string, action: number, state: boolean)
    for i = 1, #ACTIONS do
        if i == action then
            getPaddleByPlayer(ply).actions[ACTIONS[i]] = state
            if action == 5 and state then
               getPaddleByPlayer(ply).buffered_shot = true
            end
        end
    end
end

net.CreatePacket("PaddleSpawn", true)
net.CreatePacket("PaddleDespawn", true)

net.CreatePacket("ActionStart", true)
net.Receive("ActionStart", function(len, ply)
    local action = net.ReadInt()
    handleActionChange(ply, action, true)
end)

net.CreatePacket("ActionEnd", true)
net.Receive("ActionEnd", function(len, ply)
    local action = net.ReadInt()
    handleActionChange(ply, action, false)
end)

net.CreatePacket("PaddleMovement", false)
function sendPaddlePosition(paddle)
    local x, y = paddle.node:GetPosition()
    net.Start("PaddleMovement")
        net.WriteInt(paddle.id)
        net.WriteFloat(x)
        net.WriteFloat(y)
    net.Broadcast()
end

net.CreatePacket("BrickSpawn", true)
net.CreatePacket("BrickDespawn", true)
net.CreatePacket("BrickCleaned", true)

net.CreatePacket("BallSpawn", true)
net.CreatePacket("BallDespawn", true)
net.CreatePacket("BallMovement", false)
