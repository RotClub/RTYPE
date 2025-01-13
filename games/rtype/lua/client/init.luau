hook.Add("Tick", "testing", function(delta)
    if IsKeyPressed(65) then
        net.Start("Hello")
        net.SendToServer()
    end
end)
