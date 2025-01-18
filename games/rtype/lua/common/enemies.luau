
Game.enemies = {}

local enemiesUpdates = {}

local enemyId = 0


-- Pata-Pata

local patapatas = {}

function createPataPata(x, y, vx, vy, id)
    id = id or enemyId
    local patapata = root:CreateChild("RigidBody2D", "patapata" .. id, x, y, 32, 32, vx, vy)
    local patapataSprite = nil
    local patapataAnimation = nil
    if CLIENT then
        patapataSprite = patapata:CreateChild("Sprite2D", "patapataSprite" .. id, "assets/sprites/r-typesheet5.png")
        patapataAnimation = addAnimation("patapataAnimation", patapataSprite, 0, 0, 32, 36, 8, 1, true, 33)
        patapataSprite:SetSize(32*2, 36*2)
    end
    table.insert(Game.enemies, {
        node = patapata,
        sprite = patapataSprite,
        health = 3,
        animation = patapataAnimation,
        spawnPos = {x = x, y = y},
        id = id
    })
    table.insert(patapatas, Game.enemies[#Game.enemies])
    if SERVER then enemyId += 1 end
    debug("Pata-Pata created")
    return Game.enemies[#Game.enemies]
end

function enemiesUpdates.patapataUpdate(dt)
    table.foreach(patapatas, function(i, patapata)
        local x = patapata.node:GetPosition()
        local wave = math.sin(x / 60) * 60
        patapata.node:SetPosition(x, wave + patapata.spawnPos.y)
    end)
end


-- Global Functions

function destroyEnemy(id: number)
    table.foreach(Game.enemies, function(i, enemy)
        if enemy.id == id then
            enemy.node:Destroy()
            table.remove(Game.enemies, i)
        end
    end)
end

function getEnemyById(id: number)
    table.foreach(Game.enemies, function(i, enemy)
        if enemy.id == id then
            return enemy
        end
    end)
end

function checkEnemiesDamaged(projectiles, enemies)
    table.foreach(projectiles, function(i, projectiles)
        table.foreach(enemies, function(j, enemy)
            if projectiles.node:Collide(enemy.node) then
                enemy.health -= 1
                projectiles.node:Destroy()
                if SERVER then
                    net.Start("ProjectileEnd")
                        net.WriteInt(projectiles.id)
                        net.WriteBool(true)
                    net.Broadcast()
                end
            end
        end)
    end)
end

function enemiesUpdate(dt)
    table.foreach(Game.enemies, function(i, enemy)
        local x, y = enemy.node:GetGlobalPosition()
        if x < -100 then
            enemy.node:SetPosition(900, math.random(10, 470))
        end
        if enemy.health <= 0 then
            enemy.node:Destroy()
            table.remove(Game.enemies, i)
            if SERVER then
                net.Start("EnemyDestroyed")
                    net.WriteInt(enemy.id)
                    net.WriteBool(true)
                net.Broadcast()
            end
            return
        end
        if SERVER then
            local x, y = enemy.node:GetGlobalPosition()
            net.Start("EnemyPosition")
                net.WriteInt(enemy.id)
                net.WriteFloat(x)
                net.WriteFloat(y)
            net.Broadcast()
        end
    end)
    for i, update in enemiesUpdates do
        update(dt)
    end
end
