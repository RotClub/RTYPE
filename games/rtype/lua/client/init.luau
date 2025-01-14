
local parallax = root:CreateChild("Parallax", "parallaxBg", "assets/background.png", 3, nil)
local spriteA = root:CreateChild("Sprite2D", "spriteA", "assets/a.png")
local point = spriteA:CreateChild("Node2D", "point")

parallax:SetReferenceNode(point)
spriteA:SetPosition(100, 100)
spriteA:SetSource(127, 16, 35, 130)

hook.Add("Tick", "testing", function(delta)
    local dt = delta / 100000000
    local x, y = spriteA:GetPosition()

    -- A
    if IsKeyPressed(65) then
        x -= 7 * dt
    end
    -- D
    if IsKeyPressed(68) then
        x += 7 * dt
    end
    -- W
    if IsKeyPressed(87) then
        y -= 7 * dt
    end
    -- S
    if IsKeyPressed(83) then
        y += 7 * dt
    end
    if IsKeyJustPressed(67) then
        CycleColorBlindnessShader()
    end
    if IsKeyJustPressed(72) then
        net.Start("Hello")
        net.SendToServer()
    end
    spriteA:SetPosition(x, y)
    parallax:AddParallaxPosition(0.1, 0)
end)

net.Receive("Hello", function(len, ply)
    info("Hello from server")
    EnableFpsCounter()
end)
