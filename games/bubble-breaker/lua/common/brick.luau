function generateBrick(x_pos, y_pos, brick_nb)
    local brick_name = "brick_" .. brick_nb
    local brick = root:CreateChild("RigidBody2D", brick_name, x_pos, y_pos, Game.bricksSize.x, Game.bricksSize.y, 0, 0)
    if CLIENT then
        local brick_sprite = brick:CreateChild("Sprite2D", "brick_sprite", "assets/bricks/Colored_Purple-64x32.png")
        brick_sprite:SetSize(Game.bricksSize.x, Game.bricksSize.y)
    end
    return brick
end

function deleteBrick(brick)
    brick:Destroy()
end

function brickLine(x_pos, y_pos, nb_bricks)
    local bricks = {}
    for i = 0, nb_bricks do
        bricks[i] = generateBrick(x_pos + i * Game.bricksSize.x, y_pos, i)
    end
    return bricks
end