Game.defaultReferencePoint = root:CreateChild("Node2D", "defaultReferencePoint")
Game.defaultReferencePoint:SetPosition(0, 0)

local parallaxLayers = {
    root:CreateChild("Parallax", "parallaxBg1", "assets/sprites/background/background_1.png", 1, Game.defaultReferencePoint),
    root:CreateChild("Parallax", "parallaxBg2", "assets/sprites/background/background_2.png", 1.5, Game.defaultReferencePoint),
    root:CreateChild("Parallax", "parallaxBg3", "assets/sprites/background/background_3.png", 2, Game.defaultReferencePoint)
}

Game.localId = nil

function setParallaxReference(ref)
    for i = 1, #parallaxLayers do
        parallaxLayers[i]:SetReferenceNode(ref)
        debug("Set parallax reference to " .. ref:GetName())
    end
end

include("common/enemies.luau")
include("client/enemies.luau")
include("client/lagCompensation.luau")
include("client/animationsUtils.luau")
include("client/player/localPlayer.luau")
include("client/player/players.luau")
include("client/projectiles.luau")



spawnLocalPlayer()


hook.Add("Tick", "main", function(delta)
    local dt = delta / 1000000000
    if Game.localPlayer then
        localPlayerMovement(dt)
    end
    updateFireFlashes(dt)
    updateAnimations(dt)
    enemiesUpdate(dt)
    for i = 1, #parallaxLayers do
        parallaxLayers[i]:AddParallaxPosition(((5 * i) * (i / 2)) * dt, 0)
    end
end)
