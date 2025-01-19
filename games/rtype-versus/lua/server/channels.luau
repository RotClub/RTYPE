-- Action system
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
            getSpaceshipByPlayer(ply).actions[ACTIONS[i]] = state
            if action == 5 and state then
               getSpaceshipByPlayer(ply).buffered_shot = true
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
-- Action system

net.CreatePacket("SpaceshipSpawn", true)
net.CreatePacket("SpaceshipDespawn", true)
net.CreatePacket("SpaceshipPosition", false)

net.CreatePacket("SpaceshipShoot", true)
net.CreatePacket("ProjectileEnd", true)
net.CreatePacket("EnemySpawn", true)
net.CreatePacket("EnemyDestroyed", true)
net.CreatePacket("EnemyPosition", false)
