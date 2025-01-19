## Area2D Library
- ### GetName
    Retrieve the name of the Area2D.
    #### Prototype
    ```lua
    area2d:GetName() -> string | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local area2d = CreateChild("Area2D", "area2d", posX : number, posY : number, width : number, height : number)

    local name = area2d:GetName()
    print("Area2D name: " .. name) -- should be "area2d"
    ```
- ### SetName
    Set or change the name of the Area2D.
    #### Prototype
    ```lua
    area2d:SetName(name: string) -> nil
    ```
    #### Arguments
    `string` : the name to set. Cannot be "root"
    #### Example
    ```lua
    local area2d = root:CreateChild("Area2D", "area", 20, 20, 100, 100)

    area2d:SetName("new_area_name")
    print("New Area2D name", area2d:GetName()) -- should be "new_area_name"
    ```
- ### GetChildren
    Retrieve all child nodes of the Area2D.
    #### Prototype
    ```lua
    area2d:GetChildren() -> {Node} | nil
    ```

    #### Arguments
    `None`

    #### Example
    ```lua
    local area = root:CreateChild("Area2D", "MainArea", 0, 0, 200, 200)
    local staticBody = area:CreateChild("Area2D1", "StaticChild", 20, 20, 50, 50)
    local rigidBody = area:CreateChild("Area2D2", "RigidChild", 100, 100, 30, 30)

    local children = area:GetChildren()

    for i, child in ipairs(children) do
        print("Child " .. i .. ": " .. child:GetName()) -- should be "StaticChild" & "RigidChild"
    end
    ```

- ### GetChild
    Retrieve a child node with the specified name from the current `Area2D`.
    #### Prototype
    ```lua
    area2d:GetChild(childName: string) -> Area2D | nil
    ```
    #### Arguments
    - `string`: The name of the child node to retrieve.

    #### Example
    ```lua
    local area2d = root:CreateChild("Area2D", "parent_area", 10, 10, 100, 100)
    local child_1 = area2d:CreateChild("Area2D", "child_1", 20, 20, 50, 50)

    local child = area2d:GetChild("child_1")
    if child then
        print("Found child: " .. child:GetName()) -- should be "child_1"
    else
        print("Child not found")
    end
    ```
- ### CreateChild
    Create and add an `Area2D` node to the current node.

    #### Prototype
    ```lua
    node:CreateChild(type: string, name: string, posX: number, posY: number, width: number, height: number) -> Area2D | nil
    ```

    #### Arguments
    - `string` : The type of the child node (`"Area2D"`).
    - `string` : The name of the child node.
    - `number` : The X position of the area.
    - `number` : The Y position of the area.
    - `number` : The width of the area.
    - `number` : The height of the area.

    #### Example
    ```lua
    local root = scene:GetRoot()
    local area = root:CreateChild("Area2D", "SafeZone", 0, 0, 500, 300)

    print("Created area node with name: ", area:GetName()) -- should be "SafeZone"
    ```
- ### AddChild
    Add a child node to the current `Area2D`.
    #### Prototype
    ```lua
    area2d:AddChild(childNode: Area2D)
    ```
    #### Arguments
    - `childNode` (Area2D): The child node to add to this node.

    #### Example
    ```lua
    local parent = root:CreateChild("Area2D", "parent_area", 10, 10, 100, 100)
    local child = root:CreateChild("Area2D", "child_area", 20, 20, 50, 50)

    parent:AddChild(child)
    print(parent:GetChild("child_area"):GetName()) -- should print "child_area"
    ```

- ### GetPosition
    Retrieve the position of the current `Area2D`.
    #### Prototype
    ```lua
    area2d:GetPosition() -> number, number
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local area = root:CreateChild("Area2D", "area", 50, 60, 100, 100)

    local x, y = area:GetPosition()
    print("Position:", x, y) -- should print "Position: 50 60"
    ```

- ### SetPosition
    Sets the position of the `Area2D` in the 2D space.
    #### Prototype
    ```lua
    area2d:SetPosition(x: number, y: number)
    ```
    #### Arguments
    - `number`: The new X-coordinate of the area.
    - `number`: The new Y-coordinate of the area.

    #### Example
    ```lua
    local area2d = root:CreateChild("Area2D", "example_area", 10, 10, 100, 100)
    area2d:SetPosition(200, 150)
    print("Area2D position: ", area2d:GetPosition()) -- should print (200, 150)
    ```

- ### GetGlobalPosition
    Retrieves the global position of the current `Area2D`.
    #### Prototype
    ```lua
    area2d:GetGlobalPosition() -> (number, number)
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local area2d = scene:GetRoot():CreateChild("Area2D", "area", 100, 200, 300, 400)
    area2d:SetPosition(500, 600)

    local globalX, globalY = area2d:GetGlobalPosition()
    print("Global Position: (" .. globalX .. ", " .. globalY .. ")")
    ```

- ### ToggleCollision
    Enable or disable collision for the current `Area2D` object.
    #### Prototype
    ```lua
    area2d:ToggleCollision() -> void
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local area2d = root:CreateChild("Area2D", "my_area", 20, 20, 200, 100)
    area2d:ToggleCollision()
    print("Collision enabled for Area2D")
    area2d:ToggleCollision()
    print("Collision disabled for Area2D")
    ```

- ### IsCollisionEnabled
    Check whether collision is enabled for the `Area2D` node.
    #### Prototype
    ```lua
    area2d:IsCollisionEnabled() -> boolean
    ```
    #### Arguments
    `None`

    #### Example
    ```lua
    local area = root:CreateChild("Area2D", "area", 10, 10, 100, 100)
    local isEnabled = area:IsCollisionEnabled()

    if isEnabled then
        print("Collision is enabled for this area.")
    else
        print("Collision is disabled for this area.") -- should print this
    end
    ```

- ### Collide
    Check for a collision between the current `Area2D` and another node.
    #### Prototype
    ```lua
    area2d:Collide(otherNode: Area2D) -> boolean
    ```
    #### Arguments
    - `otherNode` (Area2D): The node to check for a collision with.

    #### Example
    ```lua
    local area1 = root:CreateChild("Area2D", "area1", 0, 0, 200, 100)
    local area2 = root:CreateChild("Area2D", "area2", 50, 50, 50, 50)

    area1:ToggleCollision()
    area2:ToggleCollision()

    if area1:Collide(area2) then
        print("Collision detected between area1 and area2")
    else
        print("No collision detected")
    end
    ```

- ### GetSize
    Retrieve the current size (width and height) of the `Area2D` node.
    #### Prototype
    ```lua
    area2d:GetSize() -> width: number, height: number
    ```
    #### Arguments
    `None`

    #### Example
    ```lua
    local area2d = scene:GetRoot():CreateChild("Area2D", "area_node", 10, 10, 200, 100)

    local width, height = area2d:GetSize()
    print("Width: " .. width .. ", Height: " .. height)  -- should print "Width: 200, Height: 100"
    ```

- ### SetSize
    Set a new size (width and height) for the `Area2D` node.
    #### Prototype
    ```lua
    area2d:SetSize(width: number, height: number)
    ```
    #### Arguments
    - `width` (number): The new width for the node.
    - `height` (number): The new height for the node.

    #### Example
    ```lua
    local area2d = scene:GetRoot():CreateChild("Area2D", "area_node", 10, 10, 200, 100)

    area2d:SetSize(300, 150)

    local width, height = area2d:GetSize()
    print("New Width: " .. width .. ", New Height: " .. height)  -- should print "New Width: 300, New Height: 150"
    ```

- ### Destroy
    Permanently deletes the Area2D and its associated resources.
    #### Prototype
    ```lua
    area2d:Destroy() -> nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local area2d = root:CreateChild("Area2D", "name", posX : number, posY : number, width : number, height : number)

    area2d:Destroy()
    -- trying access to area2d after destruction will result in an error
    ```