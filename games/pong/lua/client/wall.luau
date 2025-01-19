-- Desc: Wall generation functions
-- Arguments: size, space
-- Returns: Node2D
function generateMiddle(size, space)
    local wall_list = root:CreateChild("Node2D", "wall_list")
    local x_middle = WINDOW_WIDTH / 2
    local number_of_walls =  math.floor(WINDOW_HEIGHT / (size + space))
    local total_wall_height = number_of_walls * size
    local remaining_space = WINDOW_HEIGHT - total_wall_height
    local gap = remaining_space / (number_of_walls + 1)

    for i = 0, number_of_walls - 1 do
        local wall_part = wall_list:CreateChild("Node2D", "wall_part_" .. i)
        local wall_sprite = wall_part:CreateChild("Sprite2D", "wall_sprite_" .. i, "assets/player.png")

        local y_position = i * size + (i + 1) * gap
        wall_part:SetPosition(x_middle, y_position)
        wall_sprite:SetSize(size, size)
    end
    return wall_list
end