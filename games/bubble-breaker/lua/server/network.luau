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
