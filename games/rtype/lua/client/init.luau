hook.Add("Tick", "testing", function(delta)
    if IsKeyJustPressed(65) then
        net.Start("HelloTCP")
        net.SendToServer()
    end
end)

net.Receive("Hello", function(len, ply)
    info("Hello from server")
end)
