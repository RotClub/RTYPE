local parallaxLayers = {
    root:CreateChild("Parallax", "parallaxBg1", "assets/sprites/background/background_1.png", 1, nil),
    root:CreateChild("Parallax", "parallaxBg2", "assets/sprites/background/background_2.png", 1.5, nil),
    root:CreateChild("Parallax", "parallaxBg3", "assets/sprites/background/background_3.png", 2, nil)
}

include("client/player.luau")

for i = 1, #parallaxLayers do
    parallaxLayers[i]:SetReferenceNode(localPlayer)
end

local playerNum = 1
setPlayerColor(localPlayerSprite, playerNum)

hook.Add("Tick", "main", function(delta)
    local dt = delta / 1000000000
    localPlayerMovement(dt)
    for i = 1, #parallaxLayers do
        parallaxLayers[i]:AddParallaxPosition((5 * i) * dt, 0)
    end
end)

net.Receive("SpaceshipPosition", function(len, ply)
    local id = net.ReadInt()
    local x = net.ReadFloat()
    local y = net.ReadFloat()
    info("Received spaceship position: ", id, x, y)
end)
