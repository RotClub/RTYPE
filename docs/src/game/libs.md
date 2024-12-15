# Libraries

The engine makes use of multiple libraries to get the job done, some of those come from native LUAU.

They can be found in the LUAU documentation:
  - [Global functions](https://luau.org/library#global-functions)
  - [math](https://luau.org/library#math-library)
  - [table](https://luau.org/library#table-library)
  - [string](https://luau.org/library#string-library)
  - [coroutine](https://luau.org/library#coroutine-library)
  - [bit32](https://luau.org/library#bit32-library)
  - [utf8](https://luau.org/library#utf8-library)
  - [os](https://luau.org/library#os-library)
  - [debug](https://luau.org/library#debug-library)
  - [buffer](https://luau.org/library#buffer-library)
  - [vector](https://luau.org/library#vector-library)

## Custom libraries

We also implemented our own set of libraries to make controlling the engine easier.
  - [hook](#hook-library) - Assign logic that needs to be triggered on specific events.
  - [json](#json-library) - JSON Parser and Serializer.
  - [utils](#utils-library) - Set of functions that accomplish all sorts of stuff.

---
## Hook library
This library is used to create events and assign logic to them.

### hook.Add
Adds a hook to the global hook table.

#### Prototype
```luau
hook.Add(eventName: string, hookName: string, callback: (...any) -> any)
```

#### Arguments
- `eventName: string` Name of the event.
- `hookName: string` Name of the hook.
- `callback: (...any) -> any` Callback function that will be executed when the event is triggered.

#### Example
```lua
hook.Add("RType:InitServer", "hello_name", function(name: string)
    print("Hello " .. name)
end)
```

### hook.Remove
Removes a hook from the global hook table

#### Prototype
```luau
hook.Remove(eventName: string, hookName: string)
```

#### Arguments
- `eventName: string` Name of the event the hook is stored in.
- `hookName: string` Name of the hook to be deleted.

#### Example
```lua
hook.Remove("RType:InitServer", "hello_name")
```

### hook.Call
Calls all hooks under the eventName

#### Prototype
```luau
hook.Call(eventName: string, ...)
```

#### Arguments
- `eventName: string` Name of the event under which all hooks will get triggered.
- `...` Varargs, represent all arguments that will be passed to each hook's callback.
---
