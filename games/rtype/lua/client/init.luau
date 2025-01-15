
local parallax = root:CreateChild("Parallax", "parallaxBg", "assets/background.png", 1, nil)

include("client/player.luau")

parallax:SetReferenceNode(localPlayer)

local playerNum = 3
setPlayerColor(localPlayerSprite, playerNum)

hook.Add("Tick", "main", function(delta)
    local dt = delta / 1000000000
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
    if IsKeyJustPressed(67) then
        CycleColorBlindnessShader()
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
    parallax:AddParallaxPosition(40 * dt, 0)
end)

net.Receive("Hello", function(len, ply)
    info("Hello from server")
    EnableFpsCounter()
end)
