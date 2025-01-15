hook.Add("Pong:InitServer", "testing", function()
    print("Pong:InitServer called")
end)

hook.Add("Pong:Tick", "test", function(delta)
    print("delta time: " .. delta)
end)

print("Hello from client")