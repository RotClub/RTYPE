
net.Receive("EnemySpawn", function(len, ply)
    local enemyId = net.ReadInt()
    local x = net.ReadInt()
    local y = net.ReadInt()
    local vX = net.ReadInt()
    local vY = net.ReadInt()
    local enemy = createPataPata(x, y, vX, vY, enemyId)
end)

net.Receive("EnemyDestroyed", function(len, ply)
    local enemyId = net.ReadInt()
    local dead = net.ReadBool()
    destroyEnemy(enemyId)
end)

net.Receive("EnemyPosition", function(len, ply)
    local enemyId = net.ReadInt()
    local x = net.ReadInt()
    local y = net.ReadInt()
    local enemy = getEnemyById(enemyId)
    if not enemy then return end
    enemy.node:SetPosition(x, y)
end)
