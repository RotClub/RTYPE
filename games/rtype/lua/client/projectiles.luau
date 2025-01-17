
fireFlashes = {}

local FLASH_LIFETIME = 0.07

function createFireFlash(player)
    local x = Game.playersSize.x
    local w, h = 14 * 1.7, 12 * 1.7
    local fireFlash = player:CreateChild("Sprite2D", "fireFlash", "assets/sprites/r-typesheet1.png")
    fireFlash:SetSize(w, h)
    fireFlash:SetPosition(x, h / 2.9)
    setFlashFrame(fireFlash, 0)
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
            table.remove(fireFlashes, i)
            return
        end
        if fireFlash.lifetime > FLASH_LIFETIME / 2 then
            setFlashFrame(fireFlash.node, 1)
        end
    end)
end

net.Receive("FireFlash", function(len, ply)
    local id = net.ReadInt()
    if Game.localId == id then
        createFireFlash(localPlayer)
    else
        createFireFlash(otherPlayers[id])
    end
end)
