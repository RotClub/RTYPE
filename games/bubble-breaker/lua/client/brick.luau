include("common/game.luau")

function generateBrick(x_pos, y_pos, brick_nb)
    local brick_name = "brick_" .. brick_nb
    local brick = root:CreateChild("Node2D", brick_name)
    brick:SetPosition(x_pos, y_pos)
    local brick_sprite = brick:CreateChild("Sprite2D", "brick_sprite", "assets/bricks/Colored_Purple-64x32.png")
    brick_sprite:SetSize(Game.bricksSize.x, Game.bricksSize.y)
    brick:CreateChild("Area2D", "brick_hitbox", 0, 0, Game.bricksSize.x, Game.bricksSize.y)
end