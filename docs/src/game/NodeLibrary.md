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
