hook.Add("Tick", "testing", function(delta)
    if IsKeyJustPressed(65) then
        net.Start("Hello")
        net.SendToServer()
    end
end)
