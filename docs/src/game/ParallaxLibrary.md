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
    Set the reference node that the parallax effect will follow.
    #### Prototype
    ```lua
    parallax:SetReferenceNode(referenceNode: Node2D)
    ```
    #### Arguments
    - `referenceNode` (`Node2D`): The node to be used as a reference for the parallax effect.

    #### Example
    ```lua
    local parallax = scene:GetRoot():CreateChild("Parallax", "parallax_node", "parallax_source", 0.5, someNode2D)
    local referenceNode = scene:GetRoot():CreateChild("Node2D", "reference_node")
    
    -- Set the reference node for the parallax effect
    parallax:SetReferenceNode(referenceNode)
    ```

- ### AddParallaxPosition
    Add a position to the list of parallax positions to offset the parallax scrolling.
    #### Prototype
    ```lua
    parallax:AddParallaxPosition(x: number, y: number)
    ```
    #### Arguments
    - `number`: The horizontal (x) offset of the parallax position.
    - `number`: The vertical (y) offset of the parallax position.

    #### Example
    ```lua
    local parallax = scene:GetRoot():CreateChild("Parallax", "parallax_node", "parallax_source", 0.5, someNode2D)
    
    -- Add parallax positions for scrolling
    parallax:AddParallaxPosition(100, 200)
    parallax:AddParallaxPosition(-50, 75)
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
