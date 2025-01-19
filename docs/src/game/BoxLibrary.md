
## Box Library
- ### GetName
    Retrieve the name of the Box.
    #### Prototype
    ```lua
    box:GetName() -> string | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local box = root:CreateChild("Box", "box", posX : number, posY : number, width : number, height : number)

    local name = box:GetName()
    print("Box name: " .. name) -- should be box
    ```

- ### SetName
    Set or change the name of the Box.
    #### Prototype
    ```lua
    box:SetName(name: string) -> nil
    ```
    #### Arguments
    `string` : the name to set. Cannot be "root"
    #### Example
    ```lua
    local box = root:CreateChild("Box", "box", 50, 50, 200, 150)
    
    box:SetName("new_box_name")
    print("New Box name : ", box:GetName()) -- should be "new_box_name"
    ```

- ### GetChildren
    Retrieve all child nodes of the Box.
    #### Prototype
    ```lua
    box:GetChildren() -> {Node} | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local box = root:CreateChild("Box", "MainBox", 10, 10, 200, 100)
    local innerBox1 = box:CreateChild("Box", "ChildBox1", 20, 20, 50, 50)
    local innerBox2 = box:CreateChild("Box", "ChildBox2", 20, 20, 50, 100)

    local children = box:GetChildren()

    for i, child in ipairs(children) do
        print("Child " .. i .. ": " .. child:GetName()) -- should be "ChildBox"
    end
    ```

- ### GetChild
    Retrieve a child node with the specified name from the current `Box`.
    #### Prototype
    ```lua
    box:GetChild(childName: string) -> Box | nil
    ```
    #### Arguments
    - `string`: The name of the child node to retrieve.

    #### Example
    ```lua
    local box = root:CreateChild("Box", "parent_box", 50, 50, 200, 100)
    local child_1 = box:CreateChild("Box", "child_1", 70, 70, 50, 50)

    local child = box:GetChild("child_1")
    if child then
        print("Found child: " .. child:GetName()) -- should be "child_1"
    else
        print("Child not found")
    end
    ```

- ### CreateChild
    Create and add a `Box` node to the current node.

    #### Prototype
    ```lua
    node:CreateChild(type: string, name: string, posX: number, posY: number, width: number, height: number) -> Box | nil
    ```

    #### Arguments
    - `string` : The type of the child node (`"Box"`).
    - `string` : The name of the child node.
    - `number` : The X position of the box.
    - `number` : The Y position of the box.
    - `number` : The width of the box.
    - `number` : The height of the box.

    #### Example
    ```lua
    local root = scene:GetRoot()
    local box = root:CreateChild("Box", "UIBox", 200, 100, 300, 150)

    print("Created box node with name: ", box:GetName()) -- should be "UIBox"
    ```

- ### AddChild
    Add a child node to the current `Box`.
    #### Prototype
    ```lua
    box:AddChild(childNode: Box)
    ```
    #### Arguments
    - `childNode` (Box): The child node to add to this node.

    #### Example
    ```lua
    local parent = root:CreateChild("Box", "parent_box", 50, 50, 200, 100)
    local child = root:CreateChild("Box", "child_box", 70, 70, 50, 50)

    parent:AddChild(child)
    print(parent:GetChild("child_box"):GetName()) -- should print "child_box"
    ```

- ### GetPosition
    Retrieve the position of the current `Box`.
    #### Prototype
    ```lua
    box:GetPosition() -> number, number
    ```
    #### Arguments
    `None`

    #### Example
    ```lua
    local box = root:CreateChild("Box", "box", 15, 25, 200, 100)

    local x, y = box:GetPosition()
    print("Position:", x, y) -- should print "15, 25"
    ```

- ### SetPosition
    Sets the position of the `Box` in the 2D space.
    #### Prototype
    ```lua
    box:SetPosition(x: number, y: number)
    ```
    #### Arguments
    - `number`: The new X-coordinate of the box.
    - `number`: The new Y-coordinate of the box.

    #### Example
    ```lua
    local box = root:CreateChild("Box", "example_box", 0, 0, 100, 50)
    box:SetPosition(150, 75)
    print("Box position: ", box:GetPosition()) -- should print "150, 75"
    ```

- ### GetGlobalPosition
    Retrieves the global position of the current `Box`.
    #### Prototype
    ```lua
    box:GetGlobalPosition() -> (number, number)
    ```
    #### Arguments
    `None`

    #### Example
    ```lua
    local box = scene:GetRoot():CreateChild("Box", "my_box", 100, 150, 200, 300)
    box:SetPosition(500, 600)

    local globalX, globalY = box:GetGlobalPosition()
    print("Global Position: (" .. globalX .. ", " .. globalY .. ")")
    ```

- ### SetColor
    Set the color of the current `Box`. Using RGB
    #### Prototype
    ```lua
    box:SetColor(r, g, b)
    ```
    #### Arguments
    - `number`: Red component of the color (from 0 to 1).
    - `number`: Green component of the color (from 0 to 1).
    - `number`: Blue component of the color (from 0 to 1).
    #### Example
    ```lua
    local box = scene:GetRoot():CreateChild("Box", "myBox", 50, 50, 100, 100)
    box:SetColor(0, 0, 1)  -- Set the color to blue
    ```

- ### GetColor
    Retrieve the color of the current `Box`. Using RGB.
    #### Prototype
    ```lua
    box:GetColor() -> r, g, b
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local box = scene:GetRoot():CreateChild("Box", "myBox", 50, 50, 100, 100)
    box:SetColor(0, 0, 1)  -- Set the color to blue
    
    local r, g, b = box:GetColor()  -- Get the red, green, and blue components
    print("Red: " .. r .. ", Green: " .. g .. ", Blue: " .. b)
    ```

- ### SetAlpha
    Set the alpha (transparency) of the current `Box`.
    #### Prototype
    ```lua
    box:SetAlpha(alpha: number) -> nil
    ```
    #### Arguments
    - `alpha` (`number`): The alpha value (transparency) to set, ranging from 0 (completely transparent) to 1 (completely opaque).
    #### Example
    ```lua
    local box = scene:GetRoot():CreateChild("Box", "myBox", 50, 50, 100, 100)
    box:SetAlpha(0.3)  -- Set the transparency to 30%
    ```

- ### GetAlpha
    Retrieve the alpha (transparency) value of the current `Box`.
    #### Prototype
    ```lua
    box:GetAlpha() -> number
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local box = scene:GetRoot():CreateChild("Box", "myBox", 50, 50, 100, 100)
    box:SetAlpha(0.8)  -- Set the transparency to 80%
    local alpha = box:GetAlpha()  -- Retrieve the transparency value
    print(alpha)  -- Should print 0.8
    ```


- ### SetSize
    Set the box size.
    #### Prototype
    ```lua
    box:SetSize(width, height)
    ```

    #### Arguments
    - `number`: The width to set for the `Box`.
    - `number`: The height to set for the `Box`.

    #### Example
    ```lua
    local box = scene:GetRoot():CreateChild("Box", "myBox", 10, 10, 100, 200)
    box:SetSize(150, 300)  -- Changes the size of the box to 150x300

    print("Width: " .. width .. ", Height: " .. height)  -- Should print "Width: 150, Height: 300"
    ```

- ### GetSize
    Get the box size.
    #### Prototype
    ```lua
    box:GetSize() -> width, height
    ```

    #### Arguments
    - `None`

    #### Example
    ```lua
    local box = scene:GetRoot():CreateChild("Box", "myBox", 10, 10, 100, 200)
    local width, height = box:GetSize()
    print("Width: " .. width .. ", Height: " .. height)  -- Should print "Width: 100, Height: 200"
    ```
- ### Destroy
    Permanently deletes the Box and its associated resources.
    #### Prototype
    ```lua
    box:Destroy() -> nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local box = root:CreateChild("Box", "box", posX : number, posY : number, width : number, height : number)
    
    box:Destroy()
    -- Trying access box after destruction will result in an error
    ```

## Registered Node Type & Construction Method

|   Node Type   | Construction Method  |
| ------------- | ------------- |
|  Node         | CreateChild("Node", "name")|
|  Sprite2D     | CreateChild("Sprite2D", "name", "source")|
|  Parallax     | CreateChild("Parallax", "name", "source", scrollingVelocity : number, NodeReference)|
|  Label        | CreateChild("Label", "name", posX : number, posY : number, "text", "fontSource", fontSize : number)|
|  Box          | CreateChild("Box", "name", posX : number, posY : number, width : number, height : number)|
|  Area2D       | CreateChild("Area2D", "name", posX : number, posY : number, width : number, height : number)|
|  Node2D       | CreateChild("Node2D", "name")|
|  CollisionShape2D - Rectangle2D| CreateChild("CollisionShape2D", "name", type : "Rectangle", posX: number, posY : number, width : number, height : number)|
|  CollisionShape2D - Circle2D| CreateChild("CollisionShape2D", "name", type : "Circle", posX: number, posY : number, radius : number)|
|  RigidBody2D  | CreateChild("RigidBody2D", "name", posX : number, posY : number, width : number, height : number, velocityX : number, velocityY : number)|
|  StaticBody2D | CreateChild("StatiCBody2D", "name", posX : number, posY : number, width : number, height : number)|
