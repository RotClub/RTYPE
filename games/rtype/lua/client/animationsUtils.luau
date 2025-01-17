
animations = {}

function addAnimation(name: string, sprite, x: number, y: number, w: number, h: number, maxFrames: number, duration: number, frameDistance: number)
    if frameDistance == nil then
        frameDistance = w
    end
    animations[name] = {
        sprite = sprite,
        frameZone = {x = x, y = y, w = w, h = h},
        maxFrames = maxFrames,
        frameDistance = frameDistance,
        duration = duration,
        timer = 0
    }
    return animations[name]
end

function getAnimation(name: string)
    return animations[name]
end

function setAnimationFrame(animation, frame: number)
    local frameX = animation.frameZone.x + (frame * animation.frameDistance)
    animation.sprite:SetSource(frameX, animation.frameZone.y, animation.frameZone.w, animation.frameZone.h)
end

function setAnimationFrameByNode(node, frame: number)
    local animation = getAnimation(node:GetName())
    setAnimationFrame(animation, frame)
end

function updateAnimations(dt)
    table.foreach(animations, function(name, animation)
        animation.timer += dt
        if animation.timer > animation.duration then
            animation.timer = 0
        end
        local frame = math.floor(animation.timer / animation.duration * animation.maxFrames)
        setAnimationFrame(animation, frame)
    end)
end
