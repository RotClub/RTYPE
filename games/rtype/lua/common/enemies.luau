
Game.enemies = {}

enemiesUpdates = {}

local enemyId = 0


-- Pata-Pata

local patapatas = {}

function createPataPata(x, y, vx, vy, id)
    vx = vx or -130
    vy = vy or 0
    id = id or enemyId
    local w, h = 32*2, 36*2
    local patapata = root:CreateChild("RigidBody2D", "patapata" .. id, x, y, w, h, vx, vy)
    local patapataSprite = nil
    local patapataAnimation = nil
    if CLIENT then
        patapataSprite = patapata:CreateChild("Sprite2D", "patapataSprite" .. id, "assets/sprites/r-typesheet5.png")
        patapataAnimation = addAnimation("patapataAnimation" .. id, patapataSprite, 0, 0, 32, 36, 8, 0.6, true, 33)
        patapataSprite:SetSize(w, h)
    end
    table.insert(Game.enemies, {
        node = patapata,
        sprite = patapataSprite,
        health = 1,
        animation = patapataAnimation,
        spawnPos = {x = x, y = y},
        id = id
    })
    table.insert(patapatas, Game.enemies[#Game.enemies])
    if SERVER then enemyId += 1 end
    return Game.enemies[#Game.enemies]
end

local PATAPATA_WAVE_FREQUENCY = 40
local PATAPATA_WAVE_AMPLITUDE = 30

function enemiesUpdates.patapataUpdate(dt)
    table.foreach(patapatas, function(i, patapata)
        if patapata.node == nil then
            table.remove(patapatas, i)
            return
        end
        local x = patapata.node:GetPosition()
        local wave = math.sin(x / PATAPATA_WAVE_FREQUENCY) * PATAPATA_WAVE_AMPLITUDE
        patapata.node:SetPosition(x, wave + patapata.spawnPos.y)
    end)
end


-- Global Functions

function destroyEnemy(id: number)
    table.foreach(Game.enemies, function(i, enemy)
        if enemy.id == id then
            enemy.node:Destroy()
            enemy.node = nil
            if CLIENT then
                removeAnimation(enemy.animation)
            end
            table.remove(Game.enemies, i)
        end
    end)
end

function getEnemyById(id: number, enemies: table)
    enemies = enemies or Game.enemies
    table.foreach(enemies, function(i, enemy)
        if enemy.id == id then
            return enemy
        end
    end)
    return nil
end

function checkEnemiesDamaged(projectiles, enemies)
    table.foreach(projectiles, function(i, projectiles)
        table.foreach(enemies, function(j, enemy)
            if projectiles.node == nil then return end
            if projectiles.node:Collide(enemy.node) then
                if SERVER then
                    enemy.health -= 1
                    net.Start("ProjectileEnd")
                    net.WriteInt(projectiles.id)
                    net.WriteBool(true)
                    net.Broadcast()
                end
                projectiles.node:Destroy()
                projectiles.node = nil
                table.remove(Game.projectiles, i)
            end
        end)
    end)
end

function checkEnemiesDamagePlayers(enemies, players)
    table.foreach(enemies, function(i, enemy)
        if enemy.node == nil then return end
        table.foreach(players, function(j, player)
            if player.node == nil then return end
            if player.node:Collide(enemy.node) then
                if SERVER then
                    net.Start("SpaceshipDespawn")
                        net.WriteInt(player.id)
                        net.WriteBool(true)
                    net.Broadcast()
                end
                despawnSpaceship(player.id)
            end
        end)
    end)
end

function cleanNilEnemiesNodes(enemies)
    table.foreach(enemies, function(i, enemy)
        if enemy.node == nil then
            removeEnemy(i)
        end
    end)
end

