function defaultSpaceship()
    return {
        ["bound_player"] = nil,
        ["node"] = nil,
        ["actions"] = {
            ["MOVE_LEFT"] = false,
            ["MOVE_RIGHT"] = false,
            ["MOVE_UP"] = false,
            ["MOVE_DOWN"] = false,
            ["SHOOT"] = false
        }
    }
end

function getSpaceshipByPlayer(ply: string)
    for i = 1, #spaceships do
        if spaceships[i].bound_player == ply then
            return spaceships[i]
        end
    end
end

function spawnSpaceship(id: number, ply: string)
    spaceships[id].bound_player = ply
    spaceships[id].node
end

function syncNewPlayer(ply: string)
    for i = 1, #spaceships do
        if spaceships[i].bound_player == nil then continue end
        if spaceships[i].bound_player == ply then continue end
        net.Start("SpaceshipSpawn")
        net.WriteInt(i)
        net.SendToClient(ply)
    end
end

function sendSpaceshipPosition(i: number)
    net.Start("SpaceshipPosition")
    net.WriteInt(i)
    net.WriteFloat(spaceships[i].x)
    net.WriteFloat(spaceships[i].y)
    net.Broadcast()
end
