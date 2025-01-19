
fireFlashes = {}

local FLASH_LIFETIME = 0.07

local flashId = 0

function createProjectileWithVelocity(x, y, plrVecX, plrVecY, damage, owner, id)
    local projectile = root:CreateChild("RigidBody2D", "projectile" .. id, x, y, 16, 4, plrVecX, plrVecY)
    local projectileSprite = projectile:CreateChild("Sprite2D", "projectileSprite" .. id, "assets/sprites/r-typesheet1.png")
    projectileSprite:SetSource(249, 90, 16, 4)
    projectileSprite:SetSize(16 * 2, 4 * 2)
    table.insert(Game.projectiles, {
        node = projectile,
        sprite = projectileSprite,
        damage = damage,
        owner = owner,
        lifetime = 0,
        id = id
    })
    return Game.projectiles[#Game.projectiles]
end

function createFireFlash(player)
    local x = Game.playersSize.x
    local w, h = 14 * 1.7, 12 * 1.7
    local fireFlash = player:CreateChild("Sprite2D", "fireFlash" .. flashId, "assets/sprites/r-typesheet1.png")
    fireFlash:SetSize(w, h)
    fireFlash:SetPosition(x, h / 2.9)
    setFlashFrame(fireFlash, 0)
    flashId += 1
    table.insert(fireFlashes, {
        node = fireFlash,
        lifetime = 0
    })
end

function setFlashFrame(fireflash, frame: number)
    fireflash:SetSource(215 + (frame * 18), 85, 14, 12)
end

function updateFireFlashes(dt)
    table.foreach(fireFlashes, function(i, fireFlash)
        fireFlash.lifetime += dt
        if fireFlash.lifetime > FLASH_LIFETIME then
            fireFlash.node:Destroy()
            fireFlash.node = nil
            table.remove(fireFlashes, i)
            return
        end
        if fireFlash.lifetime > FLASH_LIFETIME / 2 then
            setFlashFrame(fireFlash.node, 1)
        end
    end)
end

net.Receive("SpaceshipShoot", function(len, ply)
    local id = net.ReadInt()
    local projectileId = net.ReadInt()
    local x = net.ReadFloat()
    local y = net.ReadFloat()
    local velX = net.ReadFloat()
    local velY = net.ReadFloat()
    if Game.localId == id then
        createProjectileWithVelocity(x, y, velX, velY, 0, Game.localPlayer, projectileId)
        createFireFlash(Game.localPlayer)
        localPlayerShootingSound:Play()
    else
        createProjectileWithVelocity(x, y, velX, velY, 0, otherPlayers[id], projectileId)
        createFireFlash(otherPlayers[id])
    end
end)

net.Receive("ProjectileEnd", function(len, ply)
    local id = net.ReadInt()
    local hit = net.ReadBool()
    local projectile = getProjectileByName("projectile" .. id)
    local proj = getProjectile(projectile)
    if proj == nil then return end
    table.remove(Game.projectiles, getProjectile(projectile))
    projectile.node:Destroy()
    projectile.node = nil
    if hit then
        -- TODO: BOOOOOOOOOOOOOOOOOM
    end
end)

