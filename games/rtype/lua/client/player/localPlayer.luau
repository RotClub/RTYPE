
localPlayer = root:CreateChild("RigidBody", "localPlayer")
localPlayerSprite = localPlayer:CreateChild("Sprite2D", "localPlayerSprite", "assets/sprites/r-typesheet42.png")

localPlayer:SetPosition(0, 0)
localPlayerSprite:SetSize(32 * 2, 14 * 2)

function explodePlayer(player)
    -- player:Destroy()
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

local LeftKey = 65
local RightKey = 68
local UpKey = 87
local DownKey = 83
local CycleKey = 67
local ShootKey = 74
local SpecialKey1 = 72

function localPlayerMovement(dt)
    local x, y = localPlayer:GetPosition()

    if IsKeyPressed(LeftKey) then
        x -= 70 * dt
    end
    if IsKeyPressed(RightKey) then
        x += 70 * dt
    end
    if IsKeyPressed(UpKey) then
        y -= 70 * dt
    end
    if IsKeyPressed(DownKey) then
        y += 70 * dt
    end
    if IsKeyJustPressed(LeftKey) then
        sendAction(1, true)
    end
    if IsKeyJustPressed(RightKey) then
        sendAction(2, true)
    end
    if IsKeyJustPressed(UpKey) then
        sendAction(3, true)
    end
    if IsKeyJustPressed(DownKey) then
        sendAction(4, true)
    end
    if IsKeyReleased(LeftKey) then
        sendAction(1, false)
    end
    if IsKeyReleased(RightKey) then
        sendAction(2, false)
    end
    if IsKeyReleased(UpKey) then
        sendAction(3, false)
    end
    if IsKeyReleased(DownKey) then
        sendAction(4, false)
    end
    if IsKeyJustPressed(CycleKey) then
        CycleColorBlindnessShader()
    end
    if IsKeyJustPressed(ShootKey) then
        sendAction(5, true)
    end
    if IsKeyReleased(ShootKey) then
        sendAction(5, false)
    end
    if IsKeyJustPressed(SpecialKey1) then
        -- net.Start("Hello")
        -- net.SendToServer()
        eplodePlayer(localPlayer)
    end
    localPlayer:SetPosition(x, y)
end
