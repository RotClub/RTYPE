
localPlayer = root:CreateChild("Node2D", "localPlayer")
localPlayerSprite = localPlayer:CreateChild("Sprite2D", "localPlayerSprite", "assets/sprites/r-typesheet42.png")

localPlayer:SetPosition(0, 0)
localPlayerSprite:SetSize(32 * 2, 14 * 2)

function setPlayerColor(playerSprite, playerNum)
    playerSprite:SetSource(67, 3 + ((playerNum - 1) * 17), 32, 14)
end

function explodePlayer(player)
    -- player:Destroy()
    local explosion = player:CreateChild("Sprite2D", "explosion", "assets/sprites/r-typesheet1.png")
    explosion:SetPosition(0, -10)
    explosion:SetSource(1, 334, 32, 28*2)
end


function localPlayerMovement(dt)
    local x, y = localPlayer:GetPosition()

    -- A
    if IsKeyPressed(65) then
        x -= 70 * dt
    end
    -- D
    if IsKeyPressed(68) then
        x += 70 * dt
    end
    -- W
    if IsKeyPressed(87) then
        y -= 70 * dt
    end
    -- S
    if IsKeyPressed(83) then
        y += 70 * dt
    end
    -- A
    if IsKeyJustPressed(65) then
        net.Start("ActionStart")
        net.WriteInt(1)
        net.SendToServer()
    end
    -- D
    if IsKeyJustPressed(68) then
        net.Start("ActionStart")
        net.WriteInt(2)
        net.SendToServer()
    end
    -- W
    if IsKeyJustPressed(87) then
        net.Start("ActionStart")
        net.WriteInt(3)
        net.SendToServer()
    end
    -- S
    if IsKeyJustPressed(83) then
        net.Start("ActionStart")
        net.WriteInt(4)
        net.SendToServer()
    end
    if IsKeyReleased(65) then
        net.Start("ActionEnd")
        net.WriteInt(1)
        net.SendToServer()
    end
    -- D
    if IsKeyReleased(68) then
        net.Start("ActionEnd")
        net.WriteInt(2)
        net.SendToServer()
    end
    -- W
    if IsKeyReleased(87) then
        net.Start("ActionEnd")
        net.WriteInt(3)
        net.SendToServer()
    end
    -- S
    if IsKeyReleased(83) then
        net.Start("ActionEnd")
        net.WriteInt(4)
        net.SendToServer()
    end
    if IsKeyJustPressed(67) then
        CycleColorBlindnessShader()
    end
    -- J
    if IsKeyJustPressed(32) then
        net.Start("ActionStart")
        net.WriteInt(5)
        net.SendToServer()
    end
    if IsKeyJustPressed(72) then
        -- net.Start("Hello")
        -- net.SendToServer()
        playerNum = playerNum + 1
        if playerNum > 4 then
            explodePlayer(localPlayer)
            playerNum = 1
        end
        setPlayerColor(localPlayerSprite, playerNum)
    end
    localPlayer:SetPosition(x, y)
end
