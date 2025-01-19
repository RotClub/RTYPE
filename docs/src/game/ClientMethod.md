## Client Luau Methods

Documentation for Luau client methods, providing functions for key events, FPS counter, color blindness modes, and window size.

The keys code is the one included in the ASCII specification. 

### IsKeyPressed
  Check if a key is currently being pressed down.

  #### Prototype
  ```lua
  IsKeyPressed(key: number) -> boolean
  ```
  
  #### Arguments
  `number`: The key code to check if it is pressed.
  
  #### Example
  ```lua
  if IsKeyPressed(KEY_SPACE) then
      print("Space key is pressed!")
  end
  ```

### IsKeyJustPressed
  Check if a key was just pressed.
  
  #### Prototype
  ```lua
  IsKeyJustPressed(key: number) -> boolean
  ```
  
  #### Arguments
  `number`: The key code to check if it was just pressed.
  
  #### Example
  ```lua
  if IsKeyJustPressed(KEY_ENTER) then
      print("Enter key was just pressed!")
  end
  ```

### IsKeyReleased
  Check if a key was just released.
  
  #### Prototype
  ```lua
  IsKeyReleased(key: number) -> boolean
  ```
  
  #### Arguments
  `number`: The key code to check if it was released.
  
  #### Example
  ```lua
  if IsKeyReleased(KEY_A) then
      print("The 'A' key was released!")
  end
  ```

### EnableFpsCounter
  Enables the FPS counter, displaying the current frame rate on the screen.
  
  #### Prototype
  ```lua
  EnableFpsCounter() -> nil
  ```
  
  #### Arguments
  `None`
  
  #### Example
  ```lua
  EnableFpsCounter()  -- Enables the FPS counter on the screen
  ```

### DisableFpsCounter
  Disables the FPS counter, removing the frame rate display from the screen.
  
  #### Prototype
  ```lua
  DisableFpsCounter() -> nil
  ```
  
  #### Arguments
  `None`
  
  #### Example
  ```lua
  DisableFpsCounter()  -- Disables the FPS counter on the screen
  ```

### DisableColorBlindnessShader
  Disables any color blindness shader or mode applied to the game.
  
  #### Prototype
  ```lua
  DisableColorBlindnessShader() -> nil
  ```
  
  #### Arguments
  `None`
  
  #### Example
  ```lua
  DisableColorBlindnessShader()  -- Disables any color blindness mode in the game
  ```

### CycleColorBlindnessShader
  Cycles through the available color blindness modes if any are enabled.
  
  #### Prototype
  ```lua
  CycleColorBlindnessShader() -> nil
  ```
  
  #### Arguments
  `None`
  
  #### Example
  ```lua
  CycleColorBlindnessShader()  -- Cycles through different color blindness modes (if any)
  ```

### WindowHeight 
  Returns the height of the game window (in pixels).
  
  #### Prototype
  ```lua
  WindowHeight() -> number
  ```

  #### Arguments
  `None`
  
  #### Example
  ```lua
  local height = WindowHeight()
  print("Window Height: " .. height)
  ```

### WindowWidth
  Returns the width of the game window (in pixels).

  #### Prototype
  ```lua
  WindowWidth() -> number
  ```

  #### Arguments
  `None`
  #### Example
  ```lua
  local width = WindowWidth()
  print("Window Width: " .. width)
  ```
