include("init.luau")

local sprite = root:AddChild("Sprite2D", "iamasprite", "image.png")
local nodetwod = root:AddChild("Node2D", "node_two")

print("PRINTING ROOT")
print(root:GetName())

print("PRINTING CHILDREN")
for i, v in ipairs(root:GetChildren()) do
    print(v:GetName())
end
print("END PRINTING CHILDREN")

print(root:GetChild("iamasprite"):GetName())
