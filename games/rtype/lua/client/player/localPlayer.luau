netLocalPlrX = 0
netLocalPlrY = 0

function spawnLocalPlayer()
    Game.localPlayer = root:CreateChild("RigidBody2D", "localPlayer", 0, 0, 0, 0)
    localPlayerPoint = Game.localPlayer:CreateChild("Node2D", "localPlayerPoint")
    localPlayerSprite = Game.localPlayer:CreateChild("Sprite2D", "localPlayerSprite", "assets/sprites/r-typesheet42.png")

    Game.localPlayer:SetPosition(0, 0)
    localPlayerSprite:SetSize(32 * 2, 14 * 2)
    debug(localPlayerSprite:GetGlobalPosition())
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

Keys = {}
Keys.LeftKey = 65
Keys.RightKey = 68
Keys.UpKey = 87
Keys.DownKey = 83
Keys.CycleKey = 67
Keys.ShootKey = 74
Keys.SpecialKey1 = 72

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
    end
    if IsKeyReleased(Keys.ShootKey) then
        sendAction(5, false)
    end
    if IsKeyJustPressed(Keys.SpecialKey1) then
        -- net.Start("Hello")
        -- net.SendToServer()
        explodePlayer(Game.localPlayer)
    end
    Game.localPlayer:SetPosition(netLocalPlrX, netLocalPlrY)
    -- Game.localPlayer:SetVelocity(direction.x * 100, direction.y * 100)
end
