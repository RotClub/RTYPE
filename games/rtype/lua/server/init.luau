net.CreatePacket("Hello", true)

net.Receive("Hello", function(len, ply)
    print("Hello from client: " .. ply)
end)

-- hook.Add("ClientDisconnected", function(ply)
--     print("Client disconnected: " .. ply)
-- end)
