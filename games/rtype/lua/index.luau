include("init.luau")

local function check_index(symbol_name, symbol_function)
    if symbol_name == '__index' then
        print("INSIDE __index")
        for a, b in pairs(symbol_function) do
            print(a, b)
            check_index(a, b)
        end
        print("END INSIDE __index")
    end
end

local function printMetatable(node)
    print(node.__name)
    for k, j in pairs(getmetatable(node).__index) do
        print(k, j)
        check_index(k, j)
    end
end

root:AddChild("Node2D", "node_two")
root:AddChild("Sprite2D", "iamasprite", "image.png")

print("\nPRINTING ROOT")
print(root.__name, root:GetName())
for k, j in pairs(getmetatable(root).__index) do
    print(k, j)
    check_index(k, j)
end
print("END PRINTING ROOT \n")

print("PRINTING CHILDREN")

for _, v in ipairs(root:GetChildren()) do
    printMetatable(v)
    print("----------")
end
print("END PRINTING CHILDREN\n")

-- Add a child Node
local node = root:AddChild("Node", "node1")

---- Directly call NodeMetaTable methods
print(node:GetName())  -- Works as expected (prints "node1")
node:SetName("new_name")
print(node:GetName())  -- Works as expected (prints "new_name")

---- Add a Node2D child
local sprite = root:AddChild("Node2D", "sprite")

-- Directly call Node2DMetaTable methods
print(sprite:GetPosition())  -- Works as expected (prints 0, 0)
sprite:SetPosition(100, 200)
print(sprite:GetPosition())  -- Works as expected (prints 100, 200)

