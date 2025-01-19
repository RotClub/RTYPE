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
    Create and add a child node to the current node. For the child references check the schema `system graph` in introduction and the valid registered node types.[registered_node](#registered-node-type--construction-method)
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
