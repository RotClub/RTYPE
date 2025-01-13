net.CreatePacket("Hello", false)

local next_tick = false
local next_ply = nil

net.Receive("Hello", function(len, ply)
    info("Received hello from: " .. ply)
    next_tick = true
    next_ply = ply
end)

hook.Add("Tick", "testing", function(delta: number)
    if not next_tick then return end
    net.Start("Hello")
    net.SendToClient(next_ply)
    next_tick = false
end)
