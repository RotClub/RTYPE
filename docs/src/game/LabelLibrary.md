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
    Set the color of the current `Label`. Using RGB.
    #### Prototype
    ```lua
    label:SetColor(r, g, b)
    ```
    #### Arguments
    - `number`: Red component of the color (from 0 to 1).
    - `number`: Green component of the color (from 0 to 1).
    - `number`: Blue component of the color (from 0 to 1).
    #### Example
    ```lua
    local label = scene:GetRoot():CreateChild("Label", "myLabel", 10, 10, "Hello World", "font.ttf", 24)
    label:SetColor(1, 0, 0)  -- Set the color to red
    ```

- ### GetColor
    Retrieve the color of the current `Label`. Using RGB.
    #### Prototype
    ```lua
    label:GetColor() -> r, g, b
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local label = scene:GetRoot():CreateChild("Label", "myLabel", 10, 10, "Hello World", "font.ttf", 24)
    label:SetColor(1, 0, 0)  -- Set the color to red
    
    local r, g, b = label:GetColor()  -- Get the red, green, and blue components
    print("Red: " .. r .. ", Green: " .. g .. ", Blue: " .. b)
    ```

- ### SetAlpha
    Set the alpha (transparency) of the current `Label`.
    #### Prototype
    ```lua
    label:SetAlpha(alpha: number) -> nil
    ```
    #### Arguments
    - `alpha` (`number`): The alpha value (transparency) to set, ranging from 0 (completely transparent) to 1 (completely opaque).
    #### Example
    ```lua
    local label = scene:GetRoot():CreateChild("Label", "myLabel", 10, 10, "Hello World", "font.ttf", 24)
    label:SetAlpha(0.5)  -- Set the transparency to 50%
    ```

- ### GetAlpha
    Retrieve the alpha (transparency) value of the current `Label`.
    #### Prototype
    ```lua
    label:GetAlpha() -> number
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local label = scene:GetRoot():CreateChild("Label", "myLabel", 10, 10, "Hello World", "font.ttf", 24)
    label:SetAlpha(0.7)  -- Set the transparency to 70%
    local alpha = label:GetAlpha()
    print(alpha)  -- Should print 0.7
    ```

- ### SetText
    Set the text of the Label.
    #### Prototype
    ```lua
    label:SetText(text)
    ```
    #### Arguments
    `string` : The text to set for the Label.
    #### Example
    ```lua
    local label = scene:GetRoot():CreateChild("Label", "myLabel", 10, 10, "Hello World", "font.ttf", 24)
    label:SetText("New Text")  -- Changes the label's text

    print("Label text : ", label:GetText()) -- should print "New text"
    ```
- ### GetText
    Get the text of the Label.
    #### Prototype
    ```lua
    label:GetText() -> string
    ```
    #### Arguments
    `None`
    #### Example
    ```lua
    local label = scene:GetRoot():CreateChild("Label", "myLabel", 10, 10, "Hello World", "font.ttf", 24)
    print(label:GetText())  -- Should print "Hello World"
    ```
- ### SetFont
    Set the path to the font source.
    #### Prototype
    ```lua
    label:SetFont(fontSource)
    ```
    #### Arguments
    `string`: The path to the font source to use for the Label.

    #### Example
    ```lua
    local label = scene:GetRoot():CreateChild("Label", "myLabel", 10, 10, "Hello World", "font.ttf", 24)
    
    label:SetFont("newFont.ttf")
    print(label:GetFont())  -- should print "newFont.ttf"
    ```
- ### GetFont
    Get the font file name.
    #### Prototype
    ```lua
    label:GetFont() -> string
    ```
    #### Arguments
    `None`

    #### Example
    ```lua
    local label = scene:GetRoot():CreateChild("Label", "myLabel", 10, 10, "Hello World", "font.ttf", 24)
    print(label:GetFont())  -- should print "font.ttf"
    ```
- ### SetFontSize
    Set the font size of the Label.
    #### Prototype
    ```lua
    label:SetFontSize(fontSize)
    ```

    #### Arguments
    `number`: font size to set.

    #### Example
    ```lua
    local label = scene:GetRoot():CreateChild("Label", "myLabel", 10, 10, "Hello World", "font.ttf", 24)
    label:SetFontSize(32)  -- Change the font size of 'label'
    ```

- ### GetFontSize
    Get the font  size of the Label.
    #### Prototype
    ```lua
    label:GetFontSize() -> number | nil
    ```
    #### Arguments
    `None`

    #### Example
    ```lua
    local label = scene:GetRoot():CreateChild("Label", "myLabel", 10, 10, "Hello World", "font.ttf", 24)

    print(label:GetFontSize())  -- should print "24"
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
