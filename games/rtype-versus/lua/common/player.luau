Game.playersSize = {}
Game.playersSize.x = 32 * 2
Game.playersSize.y = 14 * 2

SPACESHIP_SPEED = 300
RESPAWN_TIME = 3

function explodePlayer(id)
    local plyNode = nil
    if SERVER then
        local plyObj = getSpaceshipByPlayer(id)
        if plyObj == nil then return end
        plyNode = plyObj.node
        plyObj.respawn_cd = RESPAWN_TIME
    end
    if CLIENT then
        if Game.localId == id then
            plyNode = Game.localPly.node
        else
            if otherPlayers[id] == nil then return end
            plyNode = otherPlayers[id].node
        end
    end
    despawnSpaceship(id)
end

function playerHitUpdate(players)
    table.foreach(players, function(k, plr)
        table.foreach(Game.projectiles, function(i, projectile)
            if plr.node == nil then return end
            if projectile.owner == plr.node then return end
            if projectile.node:Collide(plr.node) then
                if SERVER then
                    net.Start("SpaceshipDespawn")
                        net.WriteInt(plr.id)
                        net.WriteBool(true)
                    net.Broadcast()
                    hook.Call("ProjectileEnd", projectile.id, true)
                    explodePlayer(plr.id)
                end
                destroyProjectile(projectile)
            end
        end)
    end)
end

