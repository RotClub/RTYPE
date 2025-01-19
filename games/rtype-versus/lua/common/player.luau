Game.playersSize = {}
Game.playersSize.x = 32 * 2
Game.playersSize.y = 14 * 2

SPACESHIP_SPEED = 100
RESPAWN_TIME = 3

function explodePlayer(player)
    if player == nil then return end
    if CLIENT then
        if player == Game.localPlayer then
            setParallaxReference(Game.defaultReferencePoint)
        end
    end
    player:Destroy()
end
