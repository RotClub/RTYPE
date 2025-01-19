## CollisionShape2D Library
- ### GetName
    Retrieve the name of the CollisionShape2D.
    #### Prototype
    ```lua
    collisionshape2d:GetName() -> string | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local collisionshape2d = root:CreateChild("CollisionShape2D", "clshape2d", type : "Rectangle", posX: number, posY : number, width : number, height : number)

    local name = collisionshape2d:GetName()
    print("CollisionShape2D name: " .. name) -- should be "clshape2d"
    ```
- ### SetName
    Set or change the name of the CollisionShape2D.
    #### Prototype
    ```lua
    collisionshape2d:SetName(name: string) -> nil
    ```
    #### Arguments
    `string` : the name to set. Cannot be "root"
    #### Example
    ```lua
    local collisionshape2d = root:CreateChild("CollisionShape2D", "collisionshape", "Rectangle", 10, 10, 50, 50)

    collisionshape2d:SetName("new_collisionshape_name")
    print("New CollisionShape2D name : ", collisionshape2d:GetName()) -- should be "new_collisionshape_name"
    ```
- ### GetChildren
    Retrieve all child nodes of the CollisionShape2D.
    #### Prototype
    ```lua
    collisionshape2d:GetChildren() -> {Node} | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local root = scene:GetRoot()
    local collisionCircle = root:CreateChild("CollisionShape2D", "PlayerCollider", "Circle", 100, 100, 25)

    print("Created collision shape with name: ", collisionCircle:GetName()) -- should be "PlayerCollider"
    ```
- ### GetChild
    Retrieve a child node with the specified name from the current `CollisionShape2D`.
    #### Prototype
    ```lua
    collisionshape2d:GetChild(childName: string) -> CollisionShape2D | nil
    ```
    #### Arguments
    - `string`: The name of the child node to retrieve.

    #### Example
    ```lua
    local collisionshape2d = root:CreateChild("CollisionShape2D", "parent_shape", "Rectangle", 0, 0, 100, 100)
    local child_1 = collisionshape2d:CreateChild("CollisionShape2D", "child_1", "Circle", 50, 50, 20)

    local child = collisionshape2d:GetChild("child_1")
    if child then
        print("Found child: " .. child:GetName()) -- should be "child_1"
    else
        print("Child not found")
    end
    ```
- ### CreateChild
    Create and add a child node to the current node. For the child references check the schema [graph](#system-graph) and the valid registered node types.[registered_node](#registered-node-type--construction-method)
    #### Prototype
    ```lua
    node:CreateChild(type: string, name: string, + others) -> Node | nil
    ```
    #### Arguments
    `string` : The type of the child node to create. It must correspond to a valid registered node type.
    `name` : The name of the child node to create.
    `+ others` : All the additional parameters depending of the node type you want to create.
    #### Example
    ```lua
    local root = scene:GetRoot()
    local collisionCircle = root:CreateChild("CollisionShape2D", "PlayerCollider", "Circle", 100, 100, 25)

    print("Created collision shape with name: ", collisionCircle:GetName()) -- should be "PlayerCollider"
    ```
- ### AddChild
    Add a child node to the current `CollisionShape2D`.
    #### Prototype
    ```lua
    collisionshape2d:AddChild(childNode: CollisionShape2D)
    ```
    #### Arguments
    - `childNode` (CollisionShape2D): The child node to add to this node.

    #### Example
    ```lua
    local parent = root:CreateChild("CollisionShape2D", "parent_shape", "Rectangle", 0, 0, 100, 100)
    local child = root:CreateChild("CollisionShape2D", "child_shape", "Circle", 50, 50, 20)

    parent:AddChild(child)
    print(parent:GetChild("child_shape"):GetName()) -- should print "child_shape"
    ```
- ### GetPosition
    Retrieve the position of the current `CollisionShape2D`.
    #### Prototype
    ```lua
    collisionshape2d:GetPosition() -> number, number
    ```
    #### Arguments
    `None`

    #### Example
    ```lua
    local shape = root:CreateChild("CollisionShape2D", "shape", "Circle", 15, 25, 50)

    local x, y = shape:GetPosition()
    print("Position:", x, y) -- should print "Position: 15 25"
    ```

- ### SetPosition
    Sets the position of the `CollisionShape2D` in the 2D space.
    #### Prototype
    ```lua
    collisionshape2d:SetPosition(x: number, y: number)
    ```
    #### Arguments
    - `number`: The new X-coordinate of the collision shape.
    - `number`: The new Y-coordinate of the collision shape.

    #### Example
    ```lua
    local collisionshape = root:CreateChild("CollisionShape2D", "example_shape", "Rectangle", 0, 0, 100, 100)
    collisionshape:SetPosition(50, 50)
    print("CollisionShape2D position: ", collisionshape:GetPosition()) -- should print (50, 50)
    ```

- ### GetGlobalPosition
    Retrieves the global position of the current `CollisionShape2D`.
    #### Prototype
    ```lua
    collisionshape2d:GetGlobalPosition() -> (number, number)
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local collisionshape2d = scene:GetRoot():CreateChild("CollisionShape2D", "collider", "Rectangle", 50, 50, 100, 100)
    collisionshape2d:SetPosition(200, 150)

    local globalX, globalY = collisionshape2d:GetGlobalPosition()
    print("Global Position: (" .. globalX .. ", " .. globalY .. ")")
    ```

- ### GetBoundingBox
    Retrieve the bounding box of the current `CollisionShape2D` Rectangle & Circle.
    #### Prototype
    ```lua
    collisionShape:GetBoundingBox() -> number, number, number, number
    ```
    #### Arguments
    - `None`

    #### Example
    ```lua
    local rectangleShape = root:CreateChild("CollisionShape2D", "rectangle", "Rectangle", 0, 0, 100, 50)

    local x, y, width, height = rectangleShape:GetBoundingBox()
    ```
    ```lua
    local circleShape = root:CreateChild("CollisionShape2D", "circle", "Circle", 50, 50, 25)

    local x, y, width, height = circleShape:GetBoundingBox()
    ```


- ### ToggleCollision
    Enable or disable collision for the current `CollisionShape2D` object.
    #### Prototype
    ```lua
    collisionshape2d:ToggleCollision() -> void
    ```
    #### Arguments
    `None`

    #### Example
    ```lua
    local collisionshape = root:CreateChild("CollisionShape2D", "my_shape", "Rectangle", 10, 10, 100, 50)
    collisionshape:ToggleCollision()
    print("Collision enabled for CollisionShape2D")
    collisionshape:ToggleCollision()
    print("Collision disabled for CollisionShape2D")
    ```

- ### IsCollisionEnabled
    Check whether collision is enabled for the `CollisionShape2D` node.
    #### Prototype
    ```lua
    collisionshape2d:IsCollisionEnabled() -> boolean
    ```
    #### Arguments
    `None`

    #### Example
    ```lua
    local shape = root:CreateChild("CollisionShape2D", "shape", "Rectangle", 0, 0, 100, 100)
    local isEnabled = shape:IsCollisionEnabled()

    if isEnabled then
        print("Collision is enabled for this shape.")
    else
        print("Collision is disabled for this shape.") -- should print this
    end
    ```

- ### Collide
    Check for a collision between the current `CollisionShape2D` and another node.
    #### Prototype
    ```lua
    collisionshape2d:Collide(otherNode: CollisionShape2D) -> boolean
    ```
    #### Arguments
    - `otherNode` (CollisionShape2D): The node to check for a collision with.

    #### Example
    ```lua
    local shape1 = root:CreateChild("CollisionShape2D", "shape1", "Rectangle", 0, 0, 100, 100)
    local shape2 = root:CreateChild("CollisionShape2D", "shape2", "Circle", 50, 50, 20)

    shape1:ToggleCollision()
    shape2:ToggleCollision()

    if shape1:Collide(shape2) then
        print("Collision detected between shape1 and shape2")
    else
        print("No collision detected")
    end
    ```

- ### Destroy
    Permanently deletes the CollisionShape2D and its associated resources.
    #### Prototype
    ```lua
    collisionshape2d:Destroy() -> nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local collisionshape2d = root:CreateChild("CollisionShape2D", "clshape2d", type : "Rectangle", posX: number, posY : number, width : number, height : number)

    collisionshape2d:Destroy()
    -- Trying access collisionshape2d after destruction will result in an error
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
