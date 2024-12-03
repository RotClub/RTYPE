hook = hook or {}

function hook.Add(eventName: string, hookName: string, callback: (...any) -> any)
    if not eventName or not hookName or not callback then
        return
    end

    if not hook[eventName] then
        hook[eventName] = {}
    end

    hook[eventName][hookName] = callback
end

function hook.Remove(eventName: string, hookName: string)
    if not eventName or not hookName then
        return
    end

    if hook[eventName] then
        hook[eventName][hookName] = nil
    end
end

function hook.Call(eventName: string, ...)
    if not eventName then
        return
    end

    if hook[eventName] then
        for _, callback in pairs(hook[eventName]) do
            callback(...)
        end
    end
end
