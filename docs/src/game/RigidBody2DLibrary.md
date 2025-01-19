## RigidBody2D Library
- ### GetName
    Retrieve the name of the RigidBody2D.
    #### Prototype
    ```lua
    rigidbody2d:GetName() -> string | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local rigidbody2d = root:CreateChild("RigidBody2D", "rgbd2d", posX : number, posY : number, width : number, height : number, velocityX : number, velocityY : number)

    local name = rigidbody2d:GetName()
    print("RigidBody2D name: " .. name) -- should be "rgbd2d"
    ```
- ### SetName
    Set or change the name of the RigidBody2D.
    #### Prototype
    ```lua
    rigidbody2d:SetName(name: string) -> nil
    ```
    #### Arguments
    `string` : the name to set. Cannot be "root"
    #### Example
    ```lua
    ```
- ### GetChildren
    Retrieve all child nodes of the RigidBody2D.
    #### Prototype
    ```lua
    rigidbody2d:GetChildren() -> {Node} | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local rigidbody = root:CreateChild("RigidBody2D", "MainRigidBody", 10, 20, 40, 40, 3, 3)
    local collisionShape1 = rigidbody:CreateChild("RigidBody2D", "ChildCollision", "Rectangle", 15, 15, 30, 30)
    local collisionShape2 = rigidbody:CreateChild("RigidBody2D", "ChildArea", "Rectangle", 15, 15, 30, 30)

    local children = rigidbody:GetChildren()

    for i, child in ipairs(children) do
        print("Child " .. i .. ": " .. child:GetName()) -- should be "ChildCollision" & "ChildArea"
    end
    ```
- ### GetChild
    Retrieve a child node with the specified name from the current `RigidBody2D`.
    #### Prototype
    ```lua
    rigidbody2d:GetChild(childName: string) -> RigidBody2D | nil
    ```
    #### Arguments
    - `childName` (string): The name of the child node to retrieve.

    #### Example
    ```lua
    local rigidbody2d = root:CreateChild("RigidBody2D", "parent_rigidbody", 30, 30, 100, 100, 5, 5)
    local child_1 = rigidbody2d:CreateChild("RigidBody2D", "child_1", 50, 50, 50, 50, 2, 2)

    local child = rigidbody2d:GetChild("child_1")
    if child then
        print("Found child: " .. child:GetName()) -- should be "child_1"
    else
        print("Child not found")
    end
    ```
- ### CreateChild
    Create and add a `RigidBody2D` node to the current node.

    #### Prototype
    ```lua
    node:CreateChild(type: string, name: string, posX: number, posY: number, width: number, height: number, velocityX: number, velocityY: number) -> RigidBody2D | nil
    ```

    #### Arguments
    - `string` : The type of the child node (`"RigidBody2D"`).
    - `string` : The name of the child node.
    - `number` : The X position of the rigid body.
    - `number` : The Y position of the rigid body.
    - `number` : The width of the rigid body.
    - `number` : The height of the rigid body.
    - `number` : The X component of the velocity.
    - `number` : The Y component of the velocity.

    #### Example
    ```lua
    local root = scene:GetRoot()
    local rigidBody = root:CreateChild("RigidBody2D", "FallingBox", 50, 50, 100, 100, 0, -9.8)

    print("Created rigid body with name: ", rigidBody:GetName()) -- should be "FallingBox"
    ```

- ### AddChild
    Add a child node to the current `RigidBody2D`.
    #### Prototype
    ```lua
    rigidbody2d:AddChild(childNode: RigidBody2D)
    ```
    #### Arguments
    - `childNode` (RigidBody2D): The child node to add to this node.

    #### Example
    ```lua
    local parent = root:CreateChild("RigidBody2D", "parent_rigidbody", 30, 30, 100, 100, 5, 5)
    local child = root:CreateChild("RigidBody2D", "child_rigidbody", 50, 50, 50, 50, 2, 2)

    parent:AddChild(child)
    print(parent:GetChild("child_rigidbody"):GetName()) -- should print "child_rigidbody"
    ```

- ### GetPosition
    Retrieve the position of the current `RigidBody2D`.
    #### Prototype
    ```lua
    rigidbody2d:GetPosition() -> number, number
    ```
    #### Arguments
    `None`

    #### Example
    ```lua
    local rigidbody = root:CreateChild("RigidBody2D", "rigidbody", 70, 80, 50, 50, 5, 5)

    local x, y = rigidbody:GetPosition()
    print("Position:", x, y) -- should print "Position: 70 80"
    ```

- ### SetPosition
    Sets the position of the `RigidBody2D` in the 2D space.
    #### Prototype
    ```lua
    rigidbody2d:SetPosition(x: number, y: number)
    ```
    #### Arguments
    - `number`: The new X-coordinate of the rigid body.
    - `number`: The new Y-coordinate of the rigid body.

    #### Example
    ```lua
    local rigidbody2d = root:CreateChild("RigidBody2D", "example_body", 20, 20, 50, 50, 5, 5)
    rigidbody2d:SetPosition(400, 300)
    print("RigidBody2D position: ", rigidbody2d:GetPosition()) -- should print (400, 300)
    ```

- ### GetGlobalPosition
    Retrieves the global position of the current `RigidBody2D`.
    #### Prototype
    ```lua
    rigidbody2d:GetGlobalPosition() -> (number, number)
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local rigidbody2d = scene:GetRoot():CreateChild("RigidBody2D", "rigid_body", 150, 250, 50, 50, 10, 10)
    rigidbody2d:SetPosition(700, 800)

    local globalX, globalY = rigidbody2d:GetGlobalPosition()
    print("Global Position: (" .. globalX .. ", " .. globalY .. ")")
    ```

- ### GetVelocity
    Retrieve the current velocity (X and Y components) of the `RigidBody2D` node.
    #### Prototype
    ```lua
    rigidbody2d:GetVelocity() -> velocityX: number, velocityY: number
    ```
    #### Arguments
    `None`

    #### Example
    ```lua
    local rigidbody = scene:GetRoot():CreateChild("RigidBody2D", "rigidbody_node", 10, 10, 50, 50, 100, 200)
    local velocityX, velocityY = rigidbody:GetVelocity()
    print("Velocity X: " .. velocityX .. ", Velocity Y: " .. velocityY)  -- should print "Velocity X: 100, Velocity Y: 200"
    ```

- ### SetVelocity
    Set the velocity (X and Y components) of the `RigidBody2D` node.
    #### Prototype
    ```lua
    rigidbody2d:SetVelocity(velocityX: number, velocityY: number)
    ```
    #### Arguments
    - `number`: The velocity on the X-axis.
    - `number`: The velocity on the Y-axis.

    #### Example
    ```lua
    local rigidbody = scene:GetRoot():CreateChild("RigidBody2D", "rigidbody_node", 10, 10, 50, 50, 0, 0)

    rigidbody:SetVelocity(100, 200)

    local velocityX, velocityY = rigidbody:GetVelocity()
    print("Velocity X: " .. velocityX .. ", Velocity Y: " .. velocityY)  -- should print "Velocity X: 100, Velocity Y: 200"
    ```

- ### ToggleCollision
    Enable or disable collision for the current `RigidBody2D` object.
    #### Prototype
    ```lua
    rigidbody2d:ToggleCollision() -> void
    ```
    #### Arguments
    `None`

    #### Example
    ```lua
    local rigidbody2d = root:CreateChild("RigidBody2D", "my_rigidbody", 30, 30, 150, 75, 5, 5)

    rigidbody2d:ToggleCollision()
    print("Collision enabled for RigidBody2D")

    rigidbody2d:ToggleCollision()
    print("Collision disabled for RigidBody2D")
    ```

- ### IsCollisionEnabled
    Check whether collision is enabled for the `RigidBody2D` node.
    #### Prototype
    ```lua
    rigidbody2d:IsCollisionEnabled() -> boolean
    ```
    #### Arguments
    `None`

    #### Example
    ```lua
    local body = root:CreateChild("RigidBody2D", "rigid_body", 20, 20, 50, 50, 10, 10)
    local isEnabled = body:IsCollisionEnabled()

    if isEnabled then
        print("Collision is enabled for this rigid body.")
    else
        print("Collision is disabled for this rigid body.") -- should print this
    end
    ```

- ### Collide
    Check for a collision between the current `RigidBody2D` and another node.
    #### Prototype
    ```lua
    rigidbody2d:Collide(otherNode: RigidBody2D) -> boolean
    ```
    #### Arguments
    - `otherNode` (RigidBody2D): The node to check for a collision with.

    #### Example
    ```lua
    local body1 = root:CreateChild("RigidBody2D", "body1", 10, 10, 100, 100, 5, 5)
    local body2 = root:CreateChild("RigidBody2D", "body2", 50, 50, 50, 50, -3, -3)

    body1:ToggleCollision()
    body2:ToggleCollision()

    if body1:Collide(body2) then
        print("Collision detected between body1 and body2")
    else
        print("No collision detected")
    end
    ```
- ### Destroy
    Permanently deletes the RigidBody2D and its associated resources.
    #### Prototype
    ```lua
    rigidbody2d:Destroy() -> nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local rigidbody2d = root:CreateChild("RigidBody2D", "name", posX : number, posY : number, width : number, height : number, velocityX : number, velocityY : number)
    
    rigidbody2d:Destroy()
    -- Trying access rgbd2d after destruction will result in an error
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
