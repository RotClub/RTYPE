
function setPlayerColor(playerSprite, playerNum)
    playerSprite:SetSource(67, 3 + ((playerNum - 1) * 17), 32, 14)
end

local MAX_PLAYERS = 4

-- id -> playerNode
local otherPlayers = {}

local function setOtherPlayerColor(id, playerNum)
    local player = otherPlayers[id]
    if player then
        local playerSprite = player:GetChild("playerSprite" .. id)
        setPlayerColor(playerSprite, playerNum)
    end
end

local function createPlayer(id)
    local player = root:CreateChild("Node2D", "player" .. id)
    local playerSprite = player:CreateChild("Sprite2D", "playerSprite" .. id, "assets/sprites/r-typesheet42.png")
    player:SetPosition(0, 0)
    playerSprite:SetSize(32 * 2, 14 * 2)
    otherPlayers[id] = player
end

local function updatePlayerPosition(id, x, y)
    local player = otherPlayers[id]
    if player then
        player:SetPosition(x, y)
    end
end

local function removePlayer(id)
    local player = otherPlayers[id]
    if player then
        -- player:Destroy()
        otherPlayers[id] = nil
    end
end

net.Receive("SpaceshipSpawn", function(len, ply)
    local id = net.ReadInt()
    if GAME.localId == nil then
        debug("Creating local player : " .. id)
        GAME.localId = id
        return
    end
    if GAME.localId == id then
        return
    end
    debug("Creating other player : " .. id)
    createPlayer(id)
    local len = #otherPlayers
    local playerNum = math.fmod(len, MAX_PLAYERS) + 1
    setOtherPlayerColor(id, playerNum)
end)

net.Receive("SpaceshipPosition", function(len, ply)
    local id = net.ReadInt()
    local x = net.ReadFloat()
    local y = net.ReadFloat()
    updatePlayerPosition(id, x, y)
end)
