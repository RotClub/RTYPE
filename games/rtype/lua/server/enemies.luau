
local MAX_ENEMIES = 5

local enemiesOnScreen = 0
local timeUntilNextEnemy = 0

function createEnemiesForNewPlayer(ply: string)
    for i = 1, #Game.enemies do
        local enemy = Game.enemies[i]
        local x, y = enemy.node:GetGlobalPosition()
        local vx, vy = enemy.node:GetVelocity()
        net.Start("EnemySpawn")
            net.WriteInt(enemy.id)
            net.WriteInt(x)
            net.WriteInt(y)
            net.WriteInt(vx)
            net.WriteInt(vy)
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
    if timeUntilNextEnemy <= 0 and enemiesOnScreen < MAX_ENEMIES then
        if math.random(1, 10) ~= 5 then
            return
        end
        createPataPata(900, spawnPos, -50, 0)
        net.Start("EnemySpawn")
            net.WriteInt(Game.enemies[#Game.enemies].id)
            net.WriteInt(900)
            net.WriteInt(spawnPos)
            net.WriteInt(-50)
            net.WriteInt(0)
        net.Broadcast()
        enemiesOnScreen += 1
    end
    enemiesUpdate(dt)
end

