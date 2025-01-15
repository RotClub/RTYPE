
localPlayer = root:CreateChild("Node2D", "localPlayer")
localPlayerSprite = localPlayer:CreateChild("Sprite2D", "localPlayerSprite", "assets/sprites/r-typesheet42.png")

localPlayer:SetPosition(0, 0)
localPlayerSprite:SetSize(32 * 2, 14 * 2)

function setPlayerColor(playerSprite, playerNum)
    playerSprite:SetSource(67, 3 + ((playerNum - 1) * 17), 32, 14)
end

function explodePlayer(player)
    -- player:Destroy()
    local explosion = player:CreateChild("Sprite2D", "explosion", "assets/sprites/r-typesheet1.png")
    explosion:SetPosition(0, -10)
    explosion:SetSource(1, 334, 32, 28*2)
end
