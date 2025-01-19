
function setPlayerColor(playerSprite, playerNum)
    playerSprite:SetSource(67, 3 + ((playerNum - 1) * 17), 32, 14)
end

otherPlayers = {}

local function createPlayer(id: number)
    local player = root:CreateChild("Node2D", "player" .. id)
    local playerSprite = player:CreateChild("Sprite2D", "playerSprite" .. id, "assets/sprites/r-typesheet42.png")
    player:SetPosition(0, 0)
    playerSprite:SetSize(Game.playersSize.x, Game.playersSize.y)
    setPlayerColor(playerSprite, id)
    otherPlayers[id] = player
end

local function updatePlayerPosition(id: number, x: number, y: number)
    local player = otherPlayers[id]
    if player then
        player:SetPosition(x, y)
    end
end

local function removePlayer(id)
    local player = otherPlayers[id]
    if player then
        player:Destroy()
        otherPlayers[id] = nil
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
        if Game.localId == id and Game.localPlayer then
            debug("Local player despawned")
            Game.localPlayer:Destroy()
            Game.localPlayer = nil
            return
        end
        removePlayer(id)
    end
    if Game.localId == id then
        explodePlayer(Game.localPlayer)
        Game.localPlayer = nil
        return
    end
    explodePlayer(otherPlayers[id])
end)
