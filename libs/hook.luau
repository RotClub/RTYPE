hook = {}
hook.hooks = {}

function hook.Add(eventName: string, hookName: string, callback: (...any) -> any)
    if not eventName or not hookName or not callback then
        error("hook.Add: Invalid arguments")
    end

    if not hook[eventName] then
        hook.hooks[eventName] = {}
    end

    hook.hooks[eventName][hookName] = callback
end

function hook.Remove(eventName: string, hookName: string)
    if not eventName or not hookName then
        error("hook.Remove: Invalid arguments")
    end

    if hook.hooks[eventName] then
        hook.hooks[eventName][hookName] = nil
    end
end

function hook.Call(eventName: string, ...)
    if not eventName then
        error("hook.Call: Invalid arguments")
    end

    if hook.hooks[eventName] then
        for _, callback in pairs(hook.hooks[eventName]) do
            callback(...)
        end
    end
end
