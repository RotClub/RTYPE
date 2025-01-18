Game.playersSize = {}
Game.playersSize.x = 32 * 2
Game.playersSize.y = 14 * 2

SPACESHIP_SPEED = 100
RESPAWN_TIME = 3

function explodePlayer(player)
    debug("Start Explode player")
    if player == nil then return end
    player:Destroy()
    debug("End Explode player")
end
