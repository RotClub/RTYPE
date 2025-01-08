net.CreatePacket("Hello", true)

net.Receive("Hello", function(len, ply)
    print("Hello from client: " .. ply)
end)

hook.Add("ClientConnecting", function(ply)
    print("Client connecting: " .. ply)
end)

hook.Add("ClientConnected", function(ply)
    print("Client connected: " .. ply)
end)
