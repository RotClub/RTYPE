net.CreatePacket("Hello", true)

net.Receive("Hello", function(len, ply)
    info("Hello from client: " .. ply)
end)
