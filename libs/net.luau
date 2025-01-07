net = net or {}
net.Callbacks = net.Callbacks or {}

function net.Receive(name, callback)
    net.Callbacks[name] = callback
end

function net.Call(name, len, ply)
    if net.Callbacks[name] then
        net.Callbacks[name](len, ply)
    end
end
