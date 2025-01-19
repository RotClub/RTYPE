function defaultSpaceship(id: number)
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
        respawn_cd = 0.0
    }
end

function getSpaceshipByPlayer(ply: string)
    debug("Getting spaceship by player " .. ply)
    for i = 1, #spaceships do
        if spaceships[i].bound_player == ply then
            return spaceships[i]
        end
    end
end

function spawnSpaceship(id: number, ply: string)
    spaceships[id].bound_player = ply
    local spawnX = 50 + (id - 1) * 650
    spaceships[id].node = root:CreateChild("RigidBody2D", "Spaceship" .. id, spawnX, 300, Game.playersSize.x, Game.playersSize.y, 0, 0)
end

function despawnSpaceship(id: number)
    if spaceships[id].node ~= nil then
        spaceships[id].node:Destroy()
    end
    spaceships[id].node = nil
    spaceships[id].shoot_cd = 0.0
    spaceships[id].respawn_cd = RESPAWN_TIME
    table.foreach(spaceships[id].actions, function(k, v)
        spaceships[id].actions[k] = false
    end)
end

function playerUpdate(player, dt: number)
    if player == nil then return end
    if player.respawn_cd > 0 then
        debug(player.respawn_cd)
        player.respawn_cd -= dt
        debug("Player " .. player.id .. " respawn in " .. player.respawn_cd)
        if player.respawn_cd <= 0 then
            debug("Respawning player " .. player.id)
            spawnSpaceship(player.id, player.bound_player)
        end
    end
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
    local xp, yp = spaceships[i].node:GetGlobalPosition()
    net.Start("SpaceshipPosition")
        net.WriteInt(i)
        net.WriteFloat(xp)
        net.WriteFloat(yp)
    net.Broadcast()
end

function updateSpaceshipVelocitiy(spaceship, delta)
    local vx, yx = spaceship.node:GetVelocity()
    if spaceship.actions.MOVE_LEFT then
        spaceship.node:SetVelocity(SPACESHIP_SPEED * -1.0, yx)
    end
    if spaceship.actions.MOVE_RIGHT then
        spaceship.node:SetVelocity(SPACESHIP_SPEED, yx)
    end
    if not spaceship.actions.MOVE_LEFT and not spaceship.actions.MOVE_RIGHT then
        spaceship.node:SetVelocity(0.0, yx)
    end
    vx, yx = spaceship.node:GetVelocity()
    if spaceship.actions.MOVE_UP then
        spaceship.node:SetVelocity(vx, SPACESHIP_SPEED * -1.0)
    end
    if spaceship.actions.MOVE_DOWN then
        spaceship.node:SetVelocity(vx, SPACESHIP_SPEED)
    end
    if not spaceship.actions.MOVE_UP and not spaceship.actions.MOVE_DOWN then
        spaceship.node:SetVelocity(vx, 0.0)
    end
end

function handleShooting(spaceship, delta: number)
    spaceship.shoot_cd += delta
    if spaceship.shoot_cd < 0.1 then
        return
    end
    if spaceship.buffered_shot then
        local px, py = spaceship.node:GetGlobalPosition()
        createProjectile(px, py, 10, spaceship.node, spaceship.id)
        spaceship.buffered_shot = false
        spaceship.shoot_cd = 0.0
    end
end

function spaceshipBoundaries(spaceship)
    local xp, yp = spaceship.node:GetGlobalPosition()
    if xp <= 0.0 then
        spaceship.node:SetPosition(0.2, yp)
    end
    if xp + Game.playersSize.x >= 800.0 then
        spaceship.node:SetPosition(799.8 - Game.playersSize.x, yp)
    end
    if yp <= 0.0 then
        spaceship.node:SetPosition(xp, 0.2)
    end
    if yp + Game.playersSize.y >= 600.0 then
    spaceship.node:SetPosition(xp, 599.8 - Game.playersSize.y)
    end
end

function projectileCollisions(dt)
    table.foreach(Game.projectiles, function(i, projectile)
        if projectile.node == nil then return end
        local x, y = projectile.node:GetGlobalPosition()
        local source = tonumber(string.match(projectile.name, "^(%d+)-"))
        local target = source == 1 and 2 or 1
        local targetSpaceship = spaceships[target]
        if targetSpaceship.node == nil then return end
        if projectile.node:Collide(targetSpaceship.node) then
            targetSpaceship.node:Destroy()
            targetSpaceship.node = nil
            targetSpaceship.respawn_cd = RESPAWN_TIME
            net.Start("SpaceshipDespawn")
                net.WriteInt(target)
                net.WriteBool(true)
            net.Broadcast()
            projectile.node:Destroy()
            projectile.node = nil
            table.remove(Game.projectiles, i)
            return
        end
    end)
end
