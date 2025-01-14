
local parallax = root:CreateChild("Parallax", "parallaxBg", "assets/background.png", 1, nil)
local spriteA = root:CreateChild("Sprite2D", "localPlayerSprite", "assets/sprites/r-typesheet1.png")
local point = spriteA:CreateChild("Node2D", "point")

parallax:SetReferenceNode(point)
spriteA:SetSource(101, 3, 32, 14)
spriteA:SetSize(32 * 2, 14 * 2)

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
        net.Start("Hello")
        net.SendToServer()
    end
    spriteA:SetPosition(x, y)
    parallax:AddParallaxPosition(40 * dt, 0)
end)

net.Receive("Hello", function(len, ply)
    info("Hello from server")
    EnableFpsCounter()
end)
