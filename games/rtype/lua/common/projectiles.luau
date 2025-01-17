Game.projectiles = {}

PLAYER_PROJECTILE_SPEED = 700
PROJECTILE_LIFETIME = 5.0

local projectileId = 0

function createProjectile(x, y, damage, owner, id)
    if CLIENT then projectileId = id end
    local plrVecX, plrVecY = owner:GetVelocity()
    local fX = x + Game.playersSize.x
    local fY = y + Game.playersSize.y / 2
    local projectile = root:CreateChild("RigidBody2D", "projectile" .. projectileId, fX, fY, 16, 4, PLAYER_PROJECTILE_SPEED + plrVecX, plrVecY)
    local projectileSprite = nil
    if CLIENT then
        projectileSprite = projectile:CreateChild("Sprite2D", "projectileSprite" .. id, "assets/sprites/r-typesheet1.png")
        projectileSprite:SetSource(249, 90, 16, 4)
    elseif SERVER then
        net.Start("SpaceshipShoot")
            net.WriteInt(id)
            net.WriteInt(projectileId + 1)
            net.WriteFloat(fX)
            net.WriteFloat(fY)
            net.WriteFloat(PLAYER_PROJECTILE_SPEED + plrVecX)
            net.WriteFloat(plrVecY)
        net.Broadcast()
    end
    projectileId += 1
    table.insert(Game.projectiles, {
        node = projectile,
        sprite = projectileSprite,
        damage = damage,
        owner = owner,
        lifetime = 0,
        id = projectileId
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

hook.Add("ProjectileEnd", "main", function(id: number, hit: boolean)
    net.Start("ProjectileEnd")
        net.WriteInt(id)
        net.WriteBool(hit)
    net.Broadcast()
end)

function updateProjectiles(dt)
    table.foreach(Game.projectiles, function(i, projectile)
        projectile.lifetime += dt
        if projectile.lifetime > PROJECTILE_LIFETIME then
            hook.Call("ProjectileEnd", projectile.id, false)
            projectile.node:Destroy()
            table.remove(Game.projectiles, i)
            return
        end
        if projectile.node == nil then
            hook.Call("ProjectileEnd", projectile.id, false)
            table.remove(Game.projectiles, i)
            return
        end
    end)
end

