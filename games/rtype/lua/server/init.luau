include("server/utils.luau")
include("server/channels.luau")

spaceships = {
    defaultSpaceship(),
    defaultSpaceship(),
    defaultSpaceship(),
    defaultSpaceship()
}

hook.Add("Tick", "main", function(delta)
    local dt = delta / 100000000
    for i = 1, #spaceships do
        if spaceships[i]["bound_player"] == nil then continue end
        -- updateSpaceshipVelocities(spaceships[i], dt)
        sendSpaceshipPosition(i)
    end
end)

hook.Add("PlayerJoin", "main", function(ply)
    for i = 1, #spaceships do
        if spaceships[i].bound_player == nil then
            spaceships[i].bound_player = ply
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
            net.Start("SpaceshipDespawn")
            net.WriteInt(i)
            net.Broadcast()
        end
    end
end)
