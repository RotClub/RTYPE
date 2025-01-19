
function setPlayerColor(playerSprite, playerNum)
    playerSprite:SetSource(0, (playerNum - 1) * 16, 32, 16)
end

otherPlayer = nil

local function createPlayer(id: number)
    local player = root:CreateChild("Node2D", "player" .. id)
    local playerSprite = player:CreateChild("Sprite2D", "playerSprite" .. id, "assets/sprites/r-typesheet42.png")
    player:SetPosition(0, 0)
    playerSprite:SetSize(Game.playersSize.x, Game.playersSize.y)
    setPlayerColor(playerSprite, id)
    otherPlayer = player
end

local function updatePlayerPosition(id: number, x: number, y: number)
    local player = otherPlayer
    if player then
        player:SetPosition(x, y)
    end
end

function despawnSpaceship(id)
    local player
    if Game.localId == id then
        player = Game.localPlayer
    else
        player = otherPlayer
    end
    if player then
        player:Destroy()
        otherPlayer = nil
    end
end

net.Receive("SpaceshipSpawn", function(len, ply)
    local id = net.ReadInt()
    if Game.localId == nil then
        Game.localId = id
        setPlayerColor(localPlayerSprite, id)
        return
    end
    if Game.localId == id then
        return
    end
    createPlayer(id)
end)

net.Receive("SpaceshipPosition", function(len, ply)
    local id = net.ReadInt()
    local x = net.ReadFloat()
    local y = net.ReadFloat()
    if Game.localId == id then
        Game.localPly.netX = x
        Game.localPly.netY = y
        return
    end
    updatePlayerPosition(id, x, y)
end)

net.Receive("SpaceshipDespawn", function(len, ply)
    local id = net.ReadInt()
    local dead = net.ReadBool()
    if not dead then
        despawnSpaceship(id)
    end
    if Game.localId == id then
        explodePlayer(Game.localId)
        Game.localPlayer = nil
        return
    end
    explodePlayer(otherPlayer)
end)
