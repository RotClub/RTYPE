local spriteA = root:CreateChild("Sprite2D", "localPlayerSprite", "assets/sprites/r-typesheet42.png")
local point = spriteA:CreateChild("Node2D", "point")

local parallaxLayers = {
    root:CreateChild("Parallax", "parallaxBg1", "assets/sprites/background/background_1.png", 1, nil),
    root:CreateChild("Parallax", "parallaxBg2", "assets/sprites/background/background_2.png", 1.5, nil),
    root:CreateChild("Parallax", "parallaxBg3", "assets/sprites/background/background_3.png", 2, nil)
}

for i = 1, #parallaxLayers do
    parallaxLayers[i]:SetReferenceNode(point)
end
spriteA:SetSize(32 * 2, 14 * 2)

local function setPlayerColor(playerSprite, playerNum)
    spriteA:SetSource(67, 3 + ((playerNum - 1) * 17), 32, 14)
end

local playerNum = 1
setPlayerColor(spriteA, playerNum)

hook.Add("Tick", "main", function(delta)
    local dt = delta / 1000000000
    local x, y = spriteA:GetPosition()

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
            playerNum = 1
        end
        setPlayerColor(spriteA, playerNum)
    end
    spriteA:SetPosition(x, y)
    parallaxLayers[1]:AddParallaxPosition(5 * dt, 0)
    parallaxLayers[2]:AddParallaxPosition(10 * dt, 0)
    parallaxLayers[3]:AddParallaxPosition(15 * dt, 0)
end)

net.Receive("Hello", function(len, ply)
    info("Hello from server")
    EnableFpsCounter()
end)
