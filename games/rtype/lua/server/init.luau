net.CreatePacket("HelloTCP", true)
net.CreatePacket("Hello", false)

target = nil

net.Receive("HelloTCP", function(len, ply)
    target = ply
end)

hook.Add("Tick", "test", function(delta)
    if not target then return end
    net.Start("Hello")
    net.SendToClient(target)
end)
