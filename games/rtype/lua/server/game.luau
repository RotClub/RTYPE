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

local SPACESHIP_SPEED = 50

function getSpaceshipByPlayer(ply: string)
    for i = 1, #spaceships do
        if spaceships[i].bound_player == ply then
            return spaceships[i]
        end
    end
end

function spawnSpaceship(id: number, ply: string)
    spaceships[id].bound_player = ply
    spaceships[id].node = root:CreateChild("RigidBody2D", "Spaceship" .. id, 50, 100, 0, 0)
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
    local x, y = spaceships[i].node:GetGlobalPosition()
    net.Start("SpaceshipPosition")
    net.WriteInt(i)
    net.WriteFloat(x)
    net.WriteFloat(y)
    net.Broadcast()
end

function updateSpaceshipVelocitiy(spaceship, delta)
    local x, y = spaceship.node:GetVelocity()
    if spaceship.actions["MOVE_LEFT"] then
        spaceship.node:SetVelocity(SPACESHIP_SPEED * -1, y)
    end
    if spaceship.actions["MOVE_RIGHT"] then
        spaceship.node:SetVelocity(SPACESHIP_SPEED, y)
    end
    if not spaceship.actions["MOVE_LEFT"] and not spaceship.actions["MOVE_RIGHT"] then
        spaceship.node:SetVelocity(0, y)
    end
    if spaceship.actions["MOVE_UP"] then
        spaceship.node:SetVelocity(x, SPACESHIP_SPEED * -1)
    end
    if spaceship.actions["MOVE_DOWN"] then
        spaceship.node:SetVelocity(x, SPACESHIP_SPEED * 1)
    end
    if not spaceship.actions["MOVE_UP"] and not spaceship.actions["MOVE_DOWN"] then
        spaceship.node:SetVelocity(x, 0)
    end
end
