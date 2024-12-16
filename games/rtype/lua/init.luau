hook.Add("RType:InitServer", "testing", function()
    print("RType:InitServer called")
end)

hook.Add("RType:Tick", "test", function(delta)
    -- print("delta time: " .. delta / 1_000_000_000)
end)

return "coucou"
