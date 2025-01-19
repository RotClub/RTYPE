## Node Library   
- ### GetName
    Get the name of the Node.
    #### Prototype
    ```lua
    node:GetName() -> string | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local node_1 = root:CreateChild("Node", "node_1")
    local node_name = node_1:GetName()

    print("Node name : " .. node_name) -- should be "node_1"
    ```

- ### SetName
    Set or change the name of the Node.
    #### Prototype
    ```lua
    node:SetName(name: string) -> nil
    ```
    #### Arguments
    `string` : the name to set. Cannot be "root"
    #### Example
    ```lua
    local node = root:CreateChild("Node", "node")
    print("New Node name : ", node:GetName()) -- should be "node"

    node:SetName("new_node_name")
    print("New Node name : ", node:GetName()) -- should be "new_node_name"
    ```

- ### GetChildren
    Retrieve all child nodes of the Node.
    #### Prototype
    ```lua
    node:GetChildren() -> {Node} | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local root = scene:GetRoot()
    local node_1 = root:CreateChild("Node", "node_1")
    local node_2 = root:CreateChild("Node", "node_2")
    
    local children = root:GetChildren()

    for i, child in ipairs(children) do
        print("Child " .. i .. ": " .. child:GetName()) -- should be "node_1" & "node_2"
    end 
    ```

- ### GetChild
    Retrieve a child node by its name.
    #### Prototype
    ```lua
    node:GetChild(name: string) -> Node | nil
    ```
    #### Arguments
    `string` : The name of the child node to retrieve.
    #### Example
    ```lua
    local root = scene:GetRoot()
    local node_1 = root:CreateChild("Node", "node_1")
    local node_2 = root:CreateChild("Node", "node_2")

    local child = root:GetChild("node_1")

    if child then
        print("Found child with name: " .. child:GetName()) -- should be "node_1"
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
    local child = root:CreateChild("Node")

    print("Created child node with type: " .. child:GetType()) -- should be "Node"
    ```

- ### AddChild
    #### Prototype
    ```lua
    node:AddChild(child: Node) -> nil
    ```
    #### Arguments
    `Node` : The node to add as a child of the current node.
    #### Example
    ```lua
    local root = scene:GetRoot()
    local child = root:CreateChild("Node", "child_node")
    root:AddChild(child)

    print("Child added : " .. child:GetName()) -- should be "child_node"
    ```

- ### Destroy
    Permanently deletes the current node and its associated resources.
    #### Prototype
    ```lua
    node:Destroy() -> nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local root = scene:GetRoot()
    local node_1 = root:CreateChild("Node", "node_1")
    
    node_1:Destroy()
    -- Trying to access node_1 after destruction will result in an error
    ```

## Node2D Library
- ### GetName
    Retrieve the name of the Node2D.
    #### Prototype
    ```lua
    node2d:GetName() -> string | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local node2d = root:CreateChild("Node2D", "node2d")
    
    local name = node2d:GetName()
    print("Node2D name: " .. name) -- should be "node2d"
    ```
- ### SetName
    Set or change the name of the Node2D.
    #### Prototype
    ```lua
    node2d:SetName(name: string) -> nil
    ```
    #### Arguments
    `string` : the name to set. Cannot be "root"
    #### Example
    ```lua
    local node2d = root:CreateChild("Node2D", "node2d")
    print("Node2D name : ", node2d:GetName()) -- should be "node2d"
    
    node2d:SetName("new_node2d_name")
    print("New Node2D name : ", node2d:GetName()) -- should be "new_node2d_name"
    ```
- ### GetChildren
    Retrieve all child nodes of the Node2D.
    #### Prototype
    ```lua
    node2d:GetChildren() -> {Node2D} | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local node2d = root:CreateChild("Node2D", "MainNode2D")
    local childNode1 = node2d:CreateChild("Node2D", "ChildNode2D1")
    local childNode2 = node2d:CreateChild("Sprite2D", "SpriteChild", "sprite1.png")

    local children = node2d:GetChildren()

    for i, child in ipairs(children) do
        print("Child " .. i .. ": " .. child:GetName()) -- should be "ChildNode2D1" & "SpriteChild"
    end

    ```
- ### GetChild
    Retrieve a child node with the specified name from the current `Node2D`.
    #### Prototype
    ```lua
    node2d:GetChild(childName: string) -> Node2D | nil
    ```
    #### Arguments
    - `string`: The name of the child node to retrieve.

    #### Example
    ```lua
    local node2d = root:CreateChild("Node2D", "parent_node2d")
    local child_1 = node2d:CreateChild("Node2D", "child_1")
    
    local child = node2d:GetChild("child_1")
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
    local node2d = root:CreateChild("Node2D", "MyNode2D")

    print("Created Node2D: ", node2d:GetName()) -- should be "MyNode2D"
    ```

- ### AddChild
    Add a child node to the current `Node2D`.
    #### Prototype
    ```lua
    node2d:AddChild(childNode: Node2D)
    ```
    #### Arguments
    - `childNode` (Node2D): The child node to add to this node.

    #### Example
    ```lua
    local parent = root:CreateChild("Node2D", "parent_node2d")
    local child = root:CreateChild("Node2D", "child_node2d")

    parent:AddChild(child)
    print(parent:GetChild("child_node2d"):GetName()) -- should print "child_node2d"
    ```
- ### GetPosition
    Retrieve the position of the current `Node2D`.
    #### Prototype
    ```lua
    node2d:GetPosition() -> number, number
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local node2d = root:CreateChild("CollisionShape2D", "shape", "Rectangle", 10, 20, 50, 50)

    local x, y = node2d:GetPosition()
    print("Position:", x, y) -- should print "Position: 10 20"
    ```

- ### SetPosition
    Sets the position of the `Node2D` in the 2D space.
    #### Prototype
    ```lua
    node2d:SetPosition(x: number, y: number)
    ```
    #### Arguments
    - `number`: The new X-coordinate of the node.
    - `number`: The new Y-coordinate of the node.

    #### Example
    ```lua
    local node2d = root:CreateChild("Node2D", "example_node")
    node2d:SetPosition(100, 200)
    print("Node2D position: ", node2d:GetPosition()) -- should print (100, 200)
    ```
    
- ### GetGlobalPosition
    Retrieves the global position of the current `Node2D`.
    #### Prototype
    ```lua
    node2d:GetGlobalPosition() -> (number, number)
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local node2d = scene:GetRoot():CreateChild("Node2D", "node_1")
    node2d:SetPosition(100, 200)

    local globalX, globalY = node2d:GetGlobalPosition()
    print("Global Position: (" .. globalX .. ", " .. globalY .. ")")
    ```

- ### Destroy
    Permanently deletes the Node2D and its associated resources.
    #### Prototype
    ```lua
    node2d:Destroy() -> nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local node2d = root:CreateChild("Node2D", "node2d")
    
    node2d:Destroy()
    -- Trying to access node2d after destruction will result in an error
    ```


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
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### SetSize
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
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


## Sprite2D Library
- ### GetName
    Retrieve the name of the Sprite2D.
    #### Prototype
    ```lua
    sprite2d:GetName() -> string | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local sprite2d = root:CreateChild("Sprite2D", "sprite2d", "src.png")

    local name = sprite2d:GetName()
    print("Sprite2D name: " .. name)-- should be "sprite2d"
    ```
- ### SetName
    Set or change the name of the Sprite2D.
    #### Prototype
    ```lua
    sprite2d:SetName(name: string) -> nil
    ```
    #### Arguments
    `string` : the name to set. Cannot be "root"
    #### Example
    ```lua
    local sprite2d = root:CreateChild("Sprite2D", "sprite", "sprite.png")
    
    sprite2d:SetName("new_sprite_name")
    print("New Sprite2D name : ", sprite2d:GetName()) -- should be "new_sprite_name"
    ```
- ### GetChildren
    Retrieve all child nodes of the Sprite2D.
    #### Prototype
    ```lua
    sprite2d:GetChildren() -> {Node} | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local sprite = root:CreateChild("Sprite2D", "MainSprite", "main_sprite.png")
    local labelChild = sprite:CreateChild("Sprite2D", "MainSprite1", "main_sprite.png")
    local boxChild = sprite:CreateChild("Sprite2D", "MainSprite2", "main_sprite.png")

    local children = sprite:GetChildren()

    for i, child in ipairs(children) do
        print("Child " .. i .. ": " .. child:GetName()) -- should be "MainSprite1" & "MainSprite2"
    end
    ```
- ### GetChild
    Retrieve a child node with the specified name from the current `Sprite2D`.
    #### Prototype
    ```lua
    sprite2d:GetChild(childName: string) -> Sprite2D | nil
    ```
    #### Arguments
    - `string`: The name of the child node to retrieve.

    #### Example
    ```lua
    local sprite2d = root:CreateChild("Sprite2D", "parent_sprite", "sprite.png")
    local child_1 = sprite2d:CreateChild("Sprite2D", "child_1", "child_sprite.png")

    local child = sprite2d:GetChild("child_1")
    if child then
        print("Found child: " .. child:GetName()) -- should be "child_1"
    else
        print("Child not found")
    end
    ```

- ### CreateChild
    Create and add a `Sprite2D` node to the current node.

    #### Prototype
    ```lua
    node:CreateChild(type: string, name: string, source: string) -> Sprite2D | nil
    ```

    #### Arguments
    - `string` : The type of the child node (`"Sprite2D"`).
    - `string` : The name of the child node.
    - `string` : The source of the sprite (image file path).

    #### Example
    ```lua
    local root = scene:GetRoot()
    local sprite = root:CreateChild("Sprite2D", "PlayerSprite", "player.png")

    print("Created sprite node with name: ", sprite:GetName()) -- should be "PlayerSprite"
    ```

- ### AddChild
    Add a child node to the current `Sprite2D`.
    #### Prototype
    ```lua
    sprite2d:AddChild(childNode: Sprite2D)
    ```
    #### Arguments
    - `childNode` (Sprite2D): The child node to add to this node.

    #### Example
    ```lua
    local parent = root:CreateChild("Sprite2D", "parent_sprite", "sprite.png")
    local child = root:CreateChild("Sprite2D", "child_sprite", "child_sprite.png")

    parent:AddChild(child)
    print(parent:GetChild("child_sprite"):GetName()) -- should print "child_sprite"
    ```

- ### GetPosition
    Retrieve the position of the current `Sprite2D`.
    #### Prototype
    ```lua
    sprite2d:GetPosition() -> number, number
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local sprite = root:CreateChild("Sprite2D", "sprite", "sprite.png")
    sprite:SetPosition(30, 40)

    local x, y = sprite:GetPosition()
    print("Position:", x, y) -- should print "Position: 30 40"
    ```
- ### SetPosition
    #### Prototype
    Sets the position of the `Sprite2D` in the 2D space.
    #### Prototype
    ```lua
    sprite2d:SetPosition(x: number, y: number)
    ```
    #### Arguments
    - `number`: The new X-coordinate of the sprite.
    - `number`: The new Y-coordinate of the sprite.

    #### Example
    ```lua
    local sprite2d = root:CreateChild("Sprite2D", "example_sprite", "sprite.png")
    sprite2d:SetPosition(300, 400)
    print("Sprite2D position: ", sprite2d:GetPosition()) -- should print (300, 400)
    ```
- ### GetGlobalPosition
    Retrieves the global position of the current `Sprite2D`.
    #### Prototype
    ```lua
    sprite2d:GetGlobalPosition() -> (number, number)
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local sprite2d = scene:GetRoot():CreateChild("Sprite2D", "sprite", "sprite.png")
    sprite2d:SetPosition(300, 400)

    local globalX, globalY = sprite2d:GetGlobalPosition()
    print("Global Position: (" .. globalX .. ", " .. globalY .. ")")
    ```

- ### SetTexture
    Sets the texture of the `Sprite2D`.
    #### Prototype
    ```lua
    sprite:SetTexture(texturePath: string) -> nil
    ```
    #### Arguments
    - `string`: The file path to the texture to apply to the `Sprite2D`.

    #### Example
    ```lua
    local sprite = scene:GetRoot():CreateChild("Sprite2D", "my_sprite", "default_texture.png")
    sprite:SetTexture("new_texture.png")
    ```

- ### SetSize
    Sets the size of the `Sprite2D`.
    #### Prototype
    ```lua
    sprite:SetSize(width: number, height: number) -> nil
    ```
    #### Arguments
    - `number`: The new width of the sprite.
    - `number`: The new height of the sprite.

    #### Example
    ```lua
    local sprite = scene:GetRoot():CreateChild("Sprite2D", "my_sprite", "texture.png")
    sprite:SetSize(100, 150)
    print("Sprite size updated to width = 100, height = 150")
    ```

- ### SetSource
    Sets the texture source rectangle of the `Sprite2D`. This allows defining a sub-region of the texture to use for rendering. Change the texture and not the sprite coordonates in the scene.
    #### Prototype
    ```lua
    sprite:SetSource(x: number, y: number, width: number, height: number) -> nil
    ```
    #### Arguments
    - `number`: The X-coordinate of the top-left corner of the source rectangle.
    - `number`: The Y-coordinate of the top-left corner of the source rectangle.
    - `number`: The width of the source rectangle.
    - `number`: The height of the source rectangle.

    #### Example
    ```lua
    local sprite = scene:GetRoot():CreateChild("Sprite2D", "my_sprite", "texture.png")
    sprite:SetSource(10, 20, 50, 60)

    print("")
    ```
- ### Destroy
    Permanently deletes the Sprite2D and its associated resources.
    #### Prototype
    ```lua
    sprite2d:Destroy() -> nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local sprite2d = root:CreateChild("Sprite2D", "sprite2d", "src.png")
    
    sprite2d:Destroy()
    -- trying access sprite2d after destruction will result in an error
    ```

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
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
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
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### SetVelocity
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
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


## Label Library
- ### GetName
    Retrieve the name of the Label.
    #### Prototype
    ```lua
    label:GetName() -> string | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local label = root:CreateChild("Label", "label", posX : number, posY : number, "text", "fontSource", fontSize : number)

    local name = label:GetName()
    print("Label name: " .. name) -- should be "label"
    ```
- ### SetName
    Set or change the name of the Label.
    #### Prototype
    ```lua
    label:SetName(name: string) -> nil
    ```
    #### Arguments
    `string` : the name to set. Cannot be "root"
    #### Example
    ```lua
    local label = root:CreateChild("Label", "label", 10, 10, "Hello World", "font.ttf", 24)
    
    label:SetName("new_label_name")
    print("New Label name : ", label:GetName()) -- should be "new_label_name"
    ```
- ### GetChildren
    Retrieve all child nodes of the Label.
    #### Prototype
    ```lua
    label:GetChildren() -> {Node} | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local label = root:CreateChild("Label", "MainLabel", 0, 0, "Hello World", "font.ttf", 18)
    local label1 = label:CreateChild("Label", "Label1", 10, 0, "Hello World !", "font.ttf", 18)
    local label2 = label:CreateChild("Label", "Label2", 20, 0, "Hello World ?", "font.ttf", 18)

    local children = label:GetChildren()

    for i, child in ipairs(children) do
        print("Child " .. i .. ": " .. child:GetName()) -- should be "ChildButton"
    end
    ```
- ### GetChild
    Retrieve a child node with the specified name from the current `Label`.
    #### Prototype
    ```lua
    label:GetChild(childName: string) -> Label | nil
    ```
    #### Arguments
    - `childName` (string): The name of the child node to retrieve.

    #### Example
    ```lua
    local label = root:CreateChild("Label", "parent_label", 10, 10, "Parent Text", "font.ttf", 24)
    local child_1 = label:CreateChild("Label", "child_1", 20, 20, "Child Text", "font.ttf", 18)

    local child = label:GetChild("child_1")
    if child then
        print("Found child: " .. child:GetName()) -- should be "child_1"
    else
        print("Child not found")
    end
    ```
- ### CreateChild
    ```lua
    node:CreateChild(type: string, name: string, posX: number, posY: number, text: string, fontSource: string, fontSize: number) -> Label | nil
    ```

    #### Arguments
    - `string` : The type of the child node (`"Label"`).
    - `string` : The name of the child node.
    - `number` : The X position of the label.
    - `number` : The Y position of the label.
    - `string` : The text to display.
    - `string` : The font source (file path).
    - `number` : The font size.

    #### Example
    ```lua
    local root = scene:GetRoot()
    local label = root:CreateChild("Label", "TitleLabel", 100, 50, "Welcome!", "font.ttf", 24)

    print("Created label node with name: ", label:GetName()) -- should be "TitleLabel"
    ```
- ### AddChild
    Add a child node to the current `Label`.
    #### Prototype
    ```lua
    label:AddChild(childNode: Label)
    ```
    #### Arguments
    - `childNode` (Label): The child node to add to this node.

    #### Example
    ```lua
    local parent = root:CreateChild("Label", "parent_label", 10, 10, "Parent Text", "font.ttf", 24)
    local child = root:CreateChild("Label", "child_label", 20, 20, "Child Text", "font.ttf", 18)

    parent:AddChild(child)
    print(parent:GetChild("child_label"):GetName()) -- should print "child_label"
    ```
- ### GetPosition
    Retrieve the position of the current `Label`.
    #### Prototype
    ```lua
    label:GetPosition() -> number, number
    ```
    #### Arguments
    `None`

    #### Example
    ```lua
    local label = root:CreateChild("Label", "label", 10, 20, "Hello World", "font.ttf", 24)

    local x, y = label:GetPosition()
    print("Position:", x, y) -- should print "Position: 10 20"
    ```

- ### SetPosition
    Sets the position of the `Label` in the 2D space.
    #### Prototype
    ```lua
    label:SetPosition(x: number, y: number)
    ```
    #### Arguments
    - `number`: The new X-coordinate of the label.
    - `number`: The new Y-coordinate of the label.

    #### Example
    ```lua
    local label = root:CreateChild("Label", "example_label", 10, 10, "Hello, World!", "font.ttf", 24)
    label:SetPosition(500, 400)
    print("Label position: ", label:GetPosition()) -- should print (500, 400)
    ```
- ### GetGlobalPosition
    Retrieves the global position of the current Label.
    #### Prototype
    ```lua
    label:GetGlobalPosition() -> (number, number)
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local label = scene:GetRoot():CreateChild("Label", "my_label", 50, 100, "Hello World", "Arial.ttf", 20)
    label:SetPosition(300, 400)

    local globalX, globalY = label:GetGlobalPosition()
    print("Global Position: (" .. globalX .. ", " .. globalY .. ")")
    ```
- ### SetColor
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### GetColor
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### SetAlpha
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### GetAlpha
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### SetText
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### GetText
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### SetFont
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### GetFont
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### SetFontSize
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### GetFontSize
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### Destroy
    Permanently deletes the Label and its associated resources.
    #### Prototype
    ```lua
    label:Destroy() -> nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local label = root:CreateChild("Label", "label", posX : number, posY : number, "text", "fontSource", fontSize : number)
    
    label:Destroy()
    -- Trying access collisionshape2d after destruction will result in an error
    ```


## Parallax Library
- ### GetName
    Retrieve the name of the Parallax.
    #### Prototype
    ```lua
    parallax:GetName() -> string | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local parallax = root:CreateChild("Parallax", "prlx2d", "source", scrollingVelocity : number, NodeReference)

    local name = parallax:GetName()
    print("Parallax name: " .. name) -- should be "prlx2d"
    ```
- ### SetName
    Set or change the name of the Parallax.
    #### Prototype
    ```lua
    parallax:SetName(name: string) -> nil
    ```
    #### Arguments
    `string` : the name to set. Cannot be "root"
    #### Example
    ```lua
    local parallax = root:CreateChild("Parallax", "parallax", "parallax.png", 1.5, backgroundNode)
    print("Parallax name : ", parallax:GetName()) -- should be "parallax"

    parallax:SetName("new_parallax_name")
    print("New Parallax name : ", parallax:GetName()) -- should be "new_parallax_name"
    ```
- ### GetChildren
    Retrieve all child nodes of the Parallax.
    #### Prototype
    ```lua
    parallax:GetChildren() -> {Node} | nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local parallax = root:CreateChild("Parallax", "MainParallax", "background.png", 0.2, root)
    local backgroundLayer = parallax:CreateChild("Parallax", "background.png", 0.8, root)
    local foregroundLayer = parallax:CreateChild("Parallax", "background.png", 1.5, root)

    local children = parallax:GetChildren()

    for i, child in ipairs(children) do
        print("Child " .. i .. ": " .. child:GetName()) -- should be "BackgroundLayer" & "ForegroundLayer"
    end
    ```
- ### GetChild
    Retrieve a child node with the specified name from the current `Parallax`.
    #### Prototype
    ```lua
    parallax:GetChild(childName: string) -> Parallax | nil
    ```
    #### Arguments
    - `childName` (string): The name of the child node to retrieve.

    #### Example
    ```lua
    local parallax = root:CreateChild("Parallax", "parent_parallax", "parallax.png", 2.0, backgroundNode)
    local child_1 = parallax:CreateChild("Parallax", "child_1", "child_parallax.png", 1.0, backgroundNode)

    local child = parallax:GetChild("child_1")
    if child then
        print("Found child: " .. child:GetName()) -- should be "child_1"
    else
        print("Child not found")
    end
    ```

- ### CreateChild
    Create and add a `Parallax` node to the current node.

    #### Prototype
    ```lua
    node:CreateChild(type: string, name: string, source: string, scrollingVelocity: number, nodeReference: Node) -> Parallax | nil
    ```

    #### Arguments
    - `string` : The type of the child node (`"Parallax"`).
    - `string` : The name of the child node.
    - `string` : The source of the parallax background.
    - `number` : The scrolling velocity of the parallax layer.
    - `Node` : A reference to a node that this parallax layer follows.

    #### Example
    ```lua
    local root = scene:GetRoot()
    local parallax = root:CreateChild("Parallax", "BackgroundParallax", "background.png", 1.5, root)

    print("Created parallax node with name: ", parallax:GetName()) -- should be "BackgroundParallax"
    ```
- ### AddChild
    Add a child node to the current `Parallax`.
    #### Prototype
    ```lua
    parallax:AddChild(childNode: Parallax)
    ```
    #### Arguments
    - `childNode` (Parallax): The child node to add to this node.

    #### Example
    ```lua
    local parent = root:CreateChild("Parallax", "parent_parallax", "parallax.png", 2.0, backgroundNode)
    local child = root:CreateChild("Parallax", "child_parallax", "child_parallax.png", 1.0, backgroundNode)

    parent:AddChild(child)
    print(parent:GetChild("child_parallax"):GetName()) -- should print "child_parallax"
    ```
- ### SetReferenceNode
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### AddParallaxPosition
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### Destroy
    Permanently deletes the Parallax and its associated resources.
    #### Prototype
    ```lua
    parallax:Destroy() -> nil
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local parallax = root:CreateChild("Parallax", "prlx2d", "source", scrollingVelocity : number, NodeReference)
    
    parallax:Destroy()
    -- Trying access parallax after destruction will result in an error
    ```

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
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### GetColor
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### SetAlpha
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### GetAlpha
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### SetSize
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
    ```
- ### GetSize
    #### Prototype
        ```lua
    ```

    #### Arguments
    #### Example
    ```lua
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
