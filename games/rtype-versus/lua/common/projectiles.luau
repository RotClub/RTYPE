Game.projectiles = {}

PLAYER_PROJECTILE_SPEED = 700 * 2
PROJECTILE_LIFETIME = 5.0

local projectileId = 0

function createProjectile(x, y, damage, owner, id)
    local plrVecX, plrVecY = owner:GetVelocity()
    local fX = x + (Game.playersSize.x * (2 - id)) - (20 * (id - 1))
    local fY = y + Game.playersSize.y / 2
    local projectile = root:CreateChild("RigidBody2D", id .. "-projectile-" .. projectileId, fX, fY, 16, 4, (PLAYER_PROJECTILE_SPEED * (3 - 2 * id)) + plrVecX, plrVecY)
    local projectileSprite = nil
    net.Start("SpaceshipShoot")
        net.WriteInt(id)
        net.WriteInt(projectileId + 1)
        net.WriteFloat(fX)
        net.WriteFloat(fY)
        net.WriteFloat((PLAYER_PROJECTILE_SPEED * (3 - 2 * id)) + plrVecX)
        net.WriteFloat(plrVecY)
    net.Broadcast()
    projectileId += 1
    table.insert(Game.projectiles, {
        node = projectile,
        sprite = projectileSprite,
        damage = damage,
        owner = owner,
        lifetime = 0,
        id = projectileId,
        name = id .. "-projectile-" .. projectileId
    })
    return Game.projectiles[#Game.projectiles]
end

function getProjectileByName(name: string)
    for i = 1, #Game.projectiles do
        if Game.projectiles[i].node:GetName() == name then
            return Game.projectiles[i]
        end
    end
end

function getProjectile(projectile)
    return table.find(Game.projectiles, projectile)
end

function playerShoot(player)
    local x, y = player:GetGlobalPosition()
    createProjectile(x, y, 10, player)
end

function destroyProjectile(projectile)
    projectile.node:Destroy()
    projectile.node = nil
    table.remove(Game.projectiles, getProjectile(projectile))
end

local function cleanNilNodes()
    table.foreach(Game.projectiles, function(i, projectile)
        if projectile.node == nil then
            table.remove(Game.projectiles, i)
        end
    end)
end

hook.Add("ProjectileEnd", "main", function(id: number, hit: boolean)
    net.Start("ProjectileEnd")
        net.WriteInt(id)
        net.WriteBool(hit)
    net.Broadcast()
end)

function updateProjectiles(dt)
    cleanNilNodes()
    table.foreach(Game.projectiles, function(i, projectile)
        projectile.lifetime += dt
        if projectile.lifetime > PROJECTILE_LIFETIME then
            hook.Call("ProjectileEnd", projectile.id, false)
            destroyProjectile(projectile)
            return
        end
        if projectile.node == nil then
            hook.Call("ProjectileEnd", projectile.id, false)
            table.remove(Game.projectiles, i)
            return
        end
    end)
end

function destroyProjectile(projectile)
    projectile.node:Destroy()
    projectile.node = nil
    table.remove(Game.projectiles, getProjectile(projectile))
end
