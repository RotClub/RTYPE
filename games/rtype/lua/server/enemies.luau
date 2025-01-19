
local MAX_ENEMIES = 5

local enemiesAlive = 0
local timeUntilNextEnemy = 0

function enemiesUpdate(dt)
    checkEnemiesDamaged(Game.projectiles, Game.enemies)
    table.foreach(Game.enemies, function(i, enemy)
        if enemy.node == nil then
            table.remove(Game.enemies, i)
            return
        end
        local x, y = enemy.node:GetGlobalPosition()
        if x < -100 then
            enemy.node:SetPosition(900, math.random(10, 470))
        end
        if enemy.health <= 0 then
            destroyEnemy(enemy.id)
            net.Start("EnemyDestroyed")
                net.WriteInt(enemy.id)
                net.WriteBool(true)
            net.Broadcast()
            enemiesAlive -= 1
            return
        end
        local x, y = enemy.node:GetGlobalPosition()
        net.Start("EnemyPosition")
            net.WriteInt(enemy.id)
            net.WriteFloat(x)
            net.WriteFloat(y)
        net.Broadcast()
    end)
    for i, update in enemiesUpdates do
        update(dt)
    end
    cleanNilEnemiesNodes(Game.enemies)
end

function createEnemiesForNewPlayer(ply: string)
    for i = 1, #Game.enemies do
        local enemy = Game.enemies[i]
        local x, y = enemy.node:GetGlobalPosition()
        local vx, vy = enemy.node:GetVelocity()
        net.Start("EnemySpawn")
            net.WriteInt(enemy.id)
            net.WriteFloat(x)
            net.WriteFloat(y)
            net.WriteFloat(vx)
            net.WriteFloat(vy)
        net.SendToClient(ply)
    end
end

function updateEnemies(dt)
    local spawnPos = math.random(10, 470)
    if timeUntilNextEnemy <= 0 then
        timeUntilNextEnemy = math.random(1, 3)
    end
    if timeUntilNextEnemy > 0 then
        timeUntilNextEnemy -= dt
    end
    if timeUntilNextEnemy <= 0 and enemiesAlive < MAX_ENEMIES then
        if math.random(1, 10) ~= 5 then
            return
        end
        local x = 900
        local y = spawnPos
        local enemy = createPataPata(x, y)
        local vx, vy = enemy.node:GetVelocity()
        net.Start("EnemySpawn")
            net.WriteInt(Game.enemies[#Game.enemies].id)
            net.WriteFloat(x)
            net.WriteFloat(y)
            net.WriteFloat(vx)
            net.WriteFloat(vy)
        net.Broadcast()
        enemiesAlive += 1
    end
    enemiesUpdate(dt)
    checkEnemiesDamagePlayers(Game.enemies, spaceships)
end

