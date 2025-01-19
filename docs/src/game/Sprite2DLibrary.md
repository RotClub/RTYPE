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