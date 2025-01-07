net.CreatePacket("Hello", true)

net.Receive("Hello", function(len, ply)
    print("Hello from client: " .. ply)
end)
