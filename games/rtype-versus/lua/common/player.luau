Game.playersSize = {}
Game.playersSize.x = 32 * 2
Game.playersSize.y = 14 * 2

SPACESHIP_SPEED = 300
RESPAWN_TIME = 3

function explodePlayer(player)
    if player == nil then return end
    player:Destroy()
end
