include("server/game.luau")
include("server/channels.luau")

spaceships = {
    defaultSpaceship(1),
    defaultSpaceship(2)
}

function getPlayerCount()
    local count = 0
    for i = 1, #spaceships do
        if spaceships[i].bound_player then
            count += 1
        end
    end
    return count
end

hook.Add("Tick", "main", function(delta)
    local dt = delta / 100000000
    for i = 1, #spaceships do
        if not spaceships[i].bound_player or not spaceships[i].node then continue end
        updateSpaceshipVelocitiy(spaceships[i], dt)
        handleShooting(spaceships[i], dt)
        spaceshipBoundaries(spaceships[i])
        sendSpaceshipPosition(i)
    end
    updateProjectiles(dt)
end)

hook.Add("PlayerJoin", "main", function(ply)
    for i = 1, #spaceships do
        if spaceships[i].bound_player == nil then
            spawnSpaceship(i, ply)
            net.Start("SpaceshipSpawn")
                net.WriteInt(i)
            net.SendToClient(ply)
            net.Start("SpaceshipSpawn")
                net.WriteInt(i)
            net.Broadcast()
            break
        end
    end
    syncNewPlayer(ply)
end)

hook.Add("PlayerLeave", "main", function(ply)
    for i = 1, #spaceships do
        if spaceships[i].bound_player == ply then
            spaceships[i].bound_player = nil
            if spaceships[i].node then
                spaceships[i].node:Destroy()
                spaceships[i].node = nil
            end
            table.foreach(spaceships[i].actions, function(k, v)
                spaceships[i].actions[k] = false
            end)
            net.Start("SpaceshipDespawn")
                net.WriteInt(i)
                net.WriteBool(false)
            net.Broadcast()
        end
    end
end)
