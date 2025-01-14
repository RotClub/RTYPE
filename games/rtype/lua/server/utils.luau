function defaultSpaceship()
    return {
        ["bound_player"] = nil,
        ["x"] = 0.0,
        ["y"] = 0.0,
        ["actions"] = {
            ["MOVE_LEFT"] = false,
            ["MOVE_RIGHT"] = false,
            ["MOVE_UP"] = false,
            ["MOVE_DOWN"] = false,
            ["SHOOT"] = false
        }
    }
end

function getSpaceshipByPlayer(ply)
    for i = 1, #spaceships do
        if spaceships[i].bound_player == ply then
            return spaceships[i]
        end
    end
end

function syncNewPlayer(ply)
    for i = 1, #spaceships do
        net.Start("SpaceshipSpawn")
        net.WriteInt(i)
        net.SendToClient(ply)
    end
end

function sendSpaceshipPosition(i)
    net.Start("SpaceshipPosition")
    net.WriteInt(i)
    net.WriteFloat(spaceships[i].x)
    net.WriteFloat(spaceships[i].y)
    net.Broadcast()
end
