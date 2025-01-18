
net.Receive("EnemySpawn", function(len, ply)
    local enemyId = net.ReadInt()
    local x = net.ReadFloat()
    local y = net.ReadFloat()
    local vX = net.ReadFloat() -- not used for now
    local vY = net.ReadFloat() -- not used for now
    local enemy = createPataPata(x, y, 0, 0, enemyId)
end)

net.Receive("EnemyDestroyed", function(len, ply)
    local enemyId = net.ReadInt()
    local dead = net.ReadBool()
    destroyEnemy(enemyId)
end)

net.Receive("EnemyPosition", function(len, ply)
    local enemyId = net.ReadInt()
    local x = net.ReadFloat()
    local y = net.ReadFloat()
    local enemyRcv = nil
    table.foreach(Game.enemies, function(i, enemy)
        if enemy.id == enemyId then
            enemyRcv = enemy
            return
        end
    end)
    if enemyRcv == nil then return end
    enemyRcv.node:SetPosition(x, y)
end)

function enemiesUpdate()
    checkEnemiesDamaged(Game.projectiles, Game.enemies)
end
