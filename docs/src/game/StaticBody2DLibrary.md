## StaticBody2D Library
- ### GetName
    Retrieve the name of the StaticBody2D.
    #### Prototype
    ```lua
    staticbody2d:GetName() -> string | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local staticbody2d = root:CreateChild("StatiCBody2D", "stbd2d", posX : number, posY : number, width : number, height : number)

    local name = staticbody2d:GetName()
    print("StaticBody2D name: " .. name) -- should be "stbd2d"
    ```
- ### SetName
    Set or change the name of the StaticBody2D.
    #### Prototype
    ```lua
    staticbody2d:SetName(name: string) -> nil
    ```
    #### Arguments
    `string` : the name to set. Cannot be "root"
    #### Example
    ```lua
    local staticbody2d = root:CreateChild("StaticBody2D", "staticbody", 50, 50, 100, 100)

    staticbody2d:SetName("new_staticbody_name")
    print("New StaticBody2D name : ", staticbody2d:GetName()) -- should be "new_staticbody_name"
    ```
- ### GetChildren
    Retrieve all child nodes of the StaticBody2D.
    #### Prototype
    ```lua
    staticbody2d:GetChildren() -> {Node} | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local staticBody = root:CreateChild("StaticBody2D", "MainStaticBody", 50, 50, 100, 100)
    local area1 = staticBody:CreateChild("StaticBody2D", "ChildArea1", 10, 10, 40, 40)
    local area2 = staticBody:CreateChild("StaticBody2D", "ChildArea2", 10, 10, 50, 80)

    local children = staticBody:GetChildren()

    for i, child in ipairs(children) do
        print("Child " .. i .. ": " .. child:GetName()) -- should be "ChildArea1" 1 "ChildArea2"
    end
    ```
- ### GetChild
    Retrieve a child node with the specified name from the current `StaticBody2D`.
    #### Prototype
    ```lua
    staticbody2d:GetChild(childName: string) -> StaticBody2D | nil
    ```
    #### Arguments
    - `childName` (string): The name of the child node to retrieve.

    #### Example
    ```lua
    local staticbody2d = root:CreateChild("StaticBody2D", "parent_staticbody", 20, 20, 100, 100)
    local child_1 = staticbody2d:CreateChild("StaticBody2D", "child_1", 40, 40, 50, 50)

    local child = staticbody2d:GetChild("child_1")
    if child then
        print("Found child: " .. child:GetName()) -- should be "child_1"
    else
        print("Child not found")
    end
    ```
- ### CreateChild
    Create and add a `StaticBody2D` node to the current node.

    #### Prototype
    ```lua
    node:CreateChild(type: string, name: string, posX: number, posY: number, width: number, height: number) -> StaticBody2D | nil
    ```

    #### Arguments
    - `string` : The type of the child node (`"StaticBody2D"`).
    - `string` : The name of the child node.
    - `number` : The X position of the static body.
    - `number` : The Y position of the static body.
    - `number` : The width of the static body.
    - `number` : The height of the static body.

    #### Example
    ```lua
    local root = scene:GetRoot()
    local staticBody = root:CreateChild("StaticBody2D", "Ground", 0, 0, 800, 50)

    print("Created static body with name: ", staticBody:GetName()) -- should be "Ground"
    ```
- ### AddChild
    Add a child node to the current `StaticBody2D`.
    #### Prototype
    ```lua
    staticbody2d:AddChild(childNode: StaticBody2D)
    ```
    #### Arguments
    - `childNode` (StaticBody2D): The child node to add to this node.

    #### Example
    ```lua
    local parent = root:CreateChild("StaticBody2D", "parent_staticbody", 20, 20, 100, 100)
    local child = root:CreateChild("StaticBody2D", "child_staticbody", 40, 40, 50, 50)

    parent:AddChild(child)
    print(parent:GetChild("child_staticbody"):GetName()) -- should print "child_staticbody"
    ```

- ### GetPosition
    Retrieve the position of the current `StaticBody2D`.
    #### Prototype
    ```lua
    staticbody2d:GetPosition() -> number, number
    ```
    #### Arguments
    `None`

    #### Example
    ```lua
    local staticbody = root:CreateChild("StaticBody2D", "staticbody", 90, 100, 50, 50)

    local x, y = staticbody:GetPosition()
    print("Position:", x, y) -- should print "Position: 90 100"
    ```

- ### SetPosition
    Sets the position of the `StaticBody2D` in the 2D space.
    #### Prototype
    ```lua
    staticbody2d:SetPosition(x: number, y: number)
    ```
    #### Arguments
    - `number`: The new X-coordinate of the static body.
    - `number`: The new Y-coordinate of the static body.

    #### Example
    ```lua
    local staticbody2d = root:CreateChild("StaticBody2D", "staticbody", 10, 10, 100, 50)
    staticbody2d:SetPosition(250, 250)
    print("StaticBody2D position: ", staticbody2d:GetPosition()) -- should print (250, 250)
    ```
- ### GetGlobalPosition
    Retrieves the global position of the current `StaticBody2D`.
    #### Prototype
    ```lua
    staticbody2d:GetGlobalPosition() -> (number, number)
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local staticbody2d = scene:GetRoot():CreateChild("StaticBody2D", "static_body", 50, 100, 200, 150)
    staticbody2d:SetPosition(400, 500)

    local globalX, globalY = staticbody2d:GetGlobalPosition()
    print("Global Position: (" .. globalX .. ", " .. globalY .. ")")
    ```

- ### ToggleCollision
    Enable or disable collision for the current `StaticBody2D` object.
    #### Prototype
    ```lua
    staticbody2d:ToggleCollision() -> void
    ```
    #### Arguments
    `None`

    #### Example
    ```lua
    local staticbody2d = root:CreateChild("StaticBody2D", "my_staticbody", 50, 50, 300, 150)

    staticbody2d:ToggleCollision()
    print("Collision enabled for StaticBody2D")

    staticbody2d:ToggleCollision()
    print("Collision disabled for StaticBody2D")
    ```
- ### IsCollisionEnabled
    Check whether collision is enabled for the `StaticBody2D` node.
    #### Prototype
    ```lua
    staticbody2d:IsCollisionEnabled() -> boolean
    ```
    #### Arguments
    `None`

    #### Example
    ```lua
    local staticBody = root:CreateChild("StaticBody2D", "static_body", 30, 30, 200, 100)
    local isEnabled = staticBody:IsCollisionEnabled()

    if isEnabled then
        print("Collision is enabled for this static body.")
    else
        print("Collision is disabled for this static body.") -- should print this
    end
    ```

- ### Collide
    Check for a collision between the current `StaticBody2D` and another node.
    #### Prototype
    ```lua
    staticbody2d:Collide(otherNode: StaticBody2D) -> boolean
    ```
    #### Arguments
    - `otherNode` (StaticBody2D): The node to check for a collision with.

    #### Example
    ```lua
    local static1 = root:CreateChild("StaticBody2D", "static1", 0, 0, 100, 100)
    local static2 = root:CreateChild("StaticBody2D", "static2", 50, 50, 50, 50)

    static1:ToggleCollision()
    static2:ToggleCollision()

    if static1:Collide(static2) then
        print("Collision detected between static1 and static2")
    else
        print("No collision detected")
    end
    ```

- ### Destroy
    Permanently deletes the StaticBody2D and its associated resources.
    #### Prototype
    ```lua
    staticbody2d:Destroy() -> nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local staticbody2d = root:CreateChild("StatiCBody2D", "name", posX : number, posY : number, width : number, height : number)

    staticbody2d:Destroy()
    -- Trying access stbd2d after destruction will result in an error
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
