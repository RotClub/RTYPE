bricks = {}

local function getRandomBrick()
    local bricks = {
        "Colored_Blue-64x32.png",
        "Colored_Green-64x32.png",
        "Colored_Red-64x32.png",
        "Colored_Yellow-64x32.png",
        "Colored_Purple-64x32.png",
        "Colored_Orange-64x32.png"
    }
    return bricks[math.random(1, #bricks)]
end

function generateBrick(x_pos, y_pos, brick_nb, yucky)
    local brick_name = "brick_" .. brick_nb
    local brick = root:CreateChild("RigidBody2D", brick_name, x_pos, y_pos, Game.brickSize.x, Game.brickSize.y, 0, 0)
    if CLIENT then
        local brick_sprite = brick:CreateChild("Sprite2D", "brick_sprite", "assets/bricks/" .. getRandomBrick())
        brick_sprite:SetSize(Game.brickSize.x, Game.brickSize.y)
        if yucky then
            local yucky_sprite = brick:CreateChild("Sprite2D", "yucky_sprite", "assets/bricks/yucky-64x32.png")
            yucky_sprite:SetSize(Game.brickSize.x, Game.brickSize.y)
        end
    elseif SERVER then
        if yucky then
            brick:CreateChild("Node", "yucky")
        end
        net.Start("BrickSpawn")
            net.WriteString(brick_nb)
            net.WriteFloat(x_pos)
            net.WriteFloat(y_pos)
            net.WriteBool(yucky)
        net.Broadcast()
    end
    return brick
end

function deleteBrick(brick)
    brick:Destroy()
end

function brickLine(x_pos, y_pos, nb_bricks)
    for i = 0, nb_bricks do
        local id = generateUUID()
        local is_yucky = math.random(0, 1) == 1
        bricks[id] = generateBrick(x_pos + i * Game.brickSize.x, y_pos, id, is_yucky)
    end
end
