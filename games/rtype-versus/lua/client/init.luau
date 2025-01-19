Game.defaultReferencePoint = root:CreateChild("Node2D", "defaultReferencePoint")
Game.defaultReferencePoint:SetPosition(0, 0)

local parallaxLayers = {
    root:CreateChild("Parallax", "parallaxBg1", "assets/sprites/background/background_1.png", 1, nil),
    root:CreateChild("Parallax", "parallaxBg2", "assets/sprites/background/background_2.png", 1.5, nil),
    root:CreateChild("Parallax", "parallaxBg3", "assets/sprites/background/background_3.png", 2, nil)
}

Game.localId = nil

include("client/lagCompensation.luau")
include("client/animationsUtils.luau")
include("client/player/localPlayer.luau")
include("client/player/players.luau")
include("client/projectiles.luau")

for i = 1, #parallaxLayers do
    parallaxLayers[i]:SetReferenceNode(Game.defaultReferencePoint)
end


spawnLocalPlayer()


hook.Add("Tick", "main", function(delta)
    local dt = delta / 1000000000
    if Game.localPlayer then
        localPlayerMovement(dt)
    end
    updateFireFlashes(dt)
    updateAnimations(dt)
    for i = 1, #parallaxLayers do
        parallaxLayers[i]:AddParallaxPosition(((5 * i) * (i / 2)) * dt, 0)
    end
    if Game.localPlayer and otherPlayer then
        local lx, ly = 0, 0
        if Game.localPlayer then
            lx, ly = Game.localPlayer:GetPosition()
        end
        local ox, oy = otherPlayer:GetPosition()
        if otherPlayer then
            ox, oy = otherPlayer:GetPosition()
        end
        Game.defaultReferencePoint:SetPosition((lx + ox) / 2, (ly + oy) / 2)
    end
end)
