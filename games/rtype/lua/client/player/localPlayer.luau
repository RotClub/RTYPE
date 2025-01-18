Game.localPly = {}
Game.localPly.netX = 0
Game.localPly.netY = 0

function spawnLocalPlayer()
    Game.localPlayer = root:CreateChild("RigidBody2D", "localPlayer", 0, 0, 0, 0, 0, 0)
    localPlayerPoint = Game.localPlayer:CreateChild("Node2D", "localPlayerPoint")
    localPlayerSprite = Game.localPlayer:CreateChild("Sprite2D", "localPlayerSprite", "assets/sprites/r-typesheet42.png")
    localPlayerArrowSprite = Game.localPlayer:CreateChild("Sprite2D", "localPlayerArrowSprite", "assets/sprites/arrow.png")
    localPlayerShootingSound = Game.localPlayer:CreateChild("SoundPlayer", "localSpaceshipShootSoundPlayer", "assets/sounds/player_shoot.mp3")

    Game.localPlayer:SetPosition(0, 0)
    localPlayerSprite:SetSize(Game.playersSize.x, Game.playersSize.y)
    localPlayerArrowSprite:SetSize(32, 32)
    localPlayerArrowSprite:SetPosition(Game.playersSize.x / 2 - 16, -30)
    localPlayerShootingSound:SetVolume(0.1)
    localPlayerShootingSound:SetPitch(1.0)
    setPlayerColor(localPlayerSprite, 1)
end

function explodePlayer(player)
    player:Destroy()
    local explosion = player:CreateChild("Sprite2D", "explosion", "assets/sprites/r-typesheet1.png")
    explosion:SetPosition(0, -10)
    explosion:SetSource(1, 334, 32, 28*2)
end

function sendAction(direction, start)
    if start then
        net.Start("ActionStart")
            net.WriteInt(direction)
        net.SendToServer()
    else
        net.Start("ActionEnd")
            net.WriteInt(direction)
        net.SendToServer()
    end
end

function calculateDistance(x1: number, y1: number, x2: number, y2: number)
    return math.sqrt((x2 - x1) ^ 2 + (y2 - y1) ^ 2)
end

Keys = {}
Keys.LeftKey = 65
Keys.RightKey = 68
Keys.UpKey = 87
Keys.DownKey = 83
Keys.CycleKey = 67
Keys.ShootKey = 74
Keys.SpecialKey1 = 72

local SHOOT_RATE = 0.1
local shootTimer = 0

function localPlayerMovement(dt)
    local direction = {}
    direction.x = 0
    direction.y = 0

    if IsKeyPressed(Keys.LeftKey) then
        direction.x = -1
    end
    if IsKeyPressed(Keys.RightKey) then
        direction.x = 1
    end
    if IsKeyPressed(Keys.UpKey) then
        direction.y = -1
    end
    if IsKeyPressed(Keys.DownKey) then
        direction.y = 1
    end
    if IsKeyJustPressed(Keys.LeftKey) then
        sendAction(1, true)
    end
    if IsKeyJustPressed(Keys.RightKey) then
        sendAction(2, true)
    end
    if IsKeyJustPressed(Keys.UpKey) then
        sendAction(3, true)
    end
    if IsKeyJustPressed(Keys.DownKey) then
        sendAction(4, true)
    end
    if IsKeyReleased(Keys.LeftKey) then
        sendAction(1, false)
    end
    if IsKeyReleased(Keys.RightKey) then
        sendAction(2, false)
    end
    if IsKeyReleased(Keys.UpKey) then
        sendAction(3, false)
    end
    if IsKeyReleased(Keys.DownKey) then
        sendAction(4, false)
    end
    if IsKeyJustPressed(Keys.CycleKey) then
        CycleColorBlindnessShader()
    end
    if IsKeyJustPressed(Keys.ShootKey) then
        sendAction(5, true)
        sendAction(5, false)
    end
    if IsKeyJustPressed(Keys.SpecialKey1) then
        -- net.Start("Hello")
        -- net.SendToServer()
        -- explodePlayer(Game.localPlayer)
    end

    Game.localPlayer:SetPosition(Game.localPly.netX, Game.localPly.netY)
end
