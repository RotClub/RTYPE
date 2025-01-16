local parallaxLayers = {
    root:CreateChild("Parallax", "parallaxBg1", "assets/sprites/background/background_1.png", 1, nil),
    root:CreateChild("Parallax", "parallaxBg2", "assets/sprites/background/background_2.png", 1.5, nil),
    root:CreateChild("Parallax", "parallaxBg3", "assets/sprites/background/background_3.png", 2, nil)
}

Game = {}

Game.localId = nil

include("client/player/localPlayer.luau")
include("client/player/players.luau")

spawnLocalPlayer(Game.localId)

for i = 1, #parallaxLayers do
    parallaxLayers[i]:SetReferenceNode(localPlayerPoint)
end

hook.Add("Tick", "main", function(delta)
    local dt = delta / 1000000000
    localPlayerMovement(dt)
    for i = 1, #parallaxLayers do
        parallaxLayers[i]:AddParallaxPosition(((5 * i) * (i / 2)) * dt, 0)
    end
end)
