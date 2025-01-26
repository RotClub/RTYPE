local bubbles = {}

local function randomVelocity()
    return math.random(-100, 100), math.random(-100, 100)
end

local function createBubble(x: number, y: number)
    local x_vel, y_vel = randomVelocity()
    local bubble = {
        node = root:CreateChild("RigidBody2D", "bubble-" .. generateUUID(), x, y, 32, 32, 0, 0),
        lifetime = math.random(500, 1500) / 1000,
        current_lifetime = 0
    }
    bubble.node:SetVelocity(x_vel, y_vel)
    bubble.node:CreateChild("Sprite2D", "bubble_sprite", "assets/bubble.png"):SetSize(32, 32)
    table.insert(bubbles, bubble)
end

function updateBubbles(delta: number)
    for i = #bubbles, 1, -1 do
        local bubble = bubbles[i]
        bubble.current_lifetime += delta
        if bubble.current_lifetime >= bubble.lifetime then
            bubble.node:Destroy()
            table.remove(bubbles, i)
        else
            local x_vel, y_vel = bubble.node:GetVelocity()
            bubble.node:SetVelocity(x_vel, y_vel - 0.1)
        end
    end
end

function bubbleExplosion(x: number, y: number)
    local amount = math.random(3, 10)
    for i = 1, amount do
        createBubble(x, y)
    end
end
