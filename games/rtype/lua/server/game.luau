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
        shoot_cd = 0.0
    }
end

local SPACESHIP_SPEED = 100

function getSpaceshipByPlayer(ply: string)
    for i = 1, #spaceships do
        if spaceships[i].bound_player == ply then
            return spaceships[i]
        end
    end
end

function spawnSpaceship(id: number, ply: string)
    spaceships[id].bound_player = ply
    spaceships[id].node = root:CreateChild("RigidBody2D", "Spaceship" .. id, 50, 100, Game.playersSize.x, Game.playersSize.y, 0, 0)
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
    -- info("sending spaceship position: " .. xp .. ", " .. yp)
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
    if spaceship.shoot_cd < PROJECTILE_LIFETIME then
        return
    end
    if spaceship.buffered_shot then
        local px, py = spaceship.node:GetGlobalPosition()
        createProjectile(px, py, 10, spaceship.node, spaceship.id)
        spaceship.buffered_shot = false
        spaceship.shoot_cd = 0.0
    end
end
