
local parallax = root:CreateChild("Parallax", "parallaxBg", "assets/background.png", 3, nil)
local spriteA = root:CreateChild("Sprite2D", "spriteA", "assets/a.png")
local point = spriteA:CreateChild("Node2D", "point")

parallax:SetReferenceNode(point)

hook.Add("Tick", "testing", function(delta)
    local dt = delta / 100000000
    local x, y = spriteA:GetPosition()
    -- A
    if IsKeyPressed(65) then
        spriteA:SetPosition(x - 7 * dt, y)
    end
    -- D
    if IsKeyPressed(68) then
        spriteA:SetPosition(x + 7 * dt, y)
    end
    -- W
    if IsKeyPressed(87) then
        spriteA:SetPosition(x, y - 7 * dt)
    end
    -- S
    if IsKeyPressed(83) then
        spriteA:SetPosition(x, y + 7 * dt)
    end
    if IsKeyJustPressed(67) then
        CycleColorBlindnessShader()
    end
    if IsKeyJustPressed(72) then
        net.Start("Hello")
        net.SendToServer()
    end
end)

net.Receive("Hello", function(len, ply)
    info("Hello from server")
    EnableFpsCounter()
end)
