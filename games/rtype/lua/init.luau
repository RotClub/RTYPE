hook.Add("RType:InitServer", "testing", function()
    print(json.encode(hook))
end)
