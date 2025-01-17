
local enemies = {}

local enemiesUpdates = {}

local enemyId = 0

-- Pata-Pata

local patapatas = {}

function createPataPata(x, y, vx, vy)
    local patapata = root:CreateChild("RigidBody2D", "patapata" .. enemyId, x, y, 32, 32, vx, vy)
    local patapataSprite = patapata:CreateChild("Sprite2D", "patapataSprite" .. enemyId, "assets/sprites/r-typesheet5.png")
    patapataSprite:SetSize(32*2, 36*2)
    local patapataAnimation = addAnimation("patapataAnimation", patapataSprite, 0, 0, 32, 36, 8, 1, 33)
    table.insert(enemies, {
        node = patapata,
        sprite = patapataSprite,
        health = 10,
        animation = patapataAnimation,
        spawnPos = {x = x, y = y},
        id = enemyId
    })
    table.insert(patapatas, enemies[#enemies])
    enemyId += 1
    return enemies[#enemies]
end


function enemiesUpdates.patapataUpdate(dt)
    table.foreach(patapatas, function(i, patapata)
        local x = patapata.node:GetPosition()
        local wave = math.sin(x / 100) * 50
        patapata.node:SetPosition(x, wave + patapata.spawnPos.y)
    end)
end

function enemiesUpdate(dt)
    table.foreach(enemies, function(i, enemy)
        if enemy.health <= 0 then
            enemy.node:Destroy()
            table.remove(enemies, i)
            return
        end
    end)
    for i, update in enemiesUpdates do
        update(dt)
    end
end

-- createPataPata(700, 100, -100, 0)
