## Server Luau Methods

### CreatePacket
  Create a new packet and register it with a given name.
  This method is usable by both client and server.
  
  When the server uses it, it create a Packet and add the Packet to it's register, sends it to all the connected clients and will send it to the future clients.

  When the client uses it, it only add the Packet to it's register.

  #### Prototype
  ```lua
  net.CreatePacket("packetName", reliable)
  ```
  #### Arguments
  `string` : The packet's name. 
  `bool` : Whether the packet is reliable or not.
  #### Example
  ```lua
  -- Creating a new packet named "PlayerData" with reliability
  net.CreatePacket("PlayerData", true)
  ```

### Start
  Start building a packet with the given name. It must be registered before.
  
  #### Prototype  
  ```lua
  net.Start("packetName")
  ```

  #### Arguments  
  `string`: The name of the packet to start.  
  
  #### Example  
  ```lua
  -- Starting the "PlayerData" packet
  net.Start("PlayerData")
  ```

### SendToClient
  Send the packet to a specific client identified by UUID. This can only be called on the server.
  
  #### Prototype  
  ```lua
  net.SendToClient("clientUUID")
  ```
  
  #### Arguments  
  `string`: The UUID of the client to send the packet to.  
  
  #### Example  
  ```lua
  -- Sending the packet to a client with a specific UUID
  net.SendToClient("abc123-xyz")
  ```


### SendToServer
  Send the built packet from the client to the server.
  
  #### Prototype  
  ```lua
  net.SendToServer()
  ```
  
  #### Arguments  
  `None`

  #### Example  
  ```lua
  -- Sending the packet from the client to the server
  net.SendToServer()
  ```

### Broadcast  
  Broadcast the built packet to all connected clients. This can only be called on the server.
  
  #### Prototype  
  ```lua
  net.Broadcast()
  ```
  
  #### Arguments  
  `None`
  
  #### Example  
  ```lua
  -- Broadcasting the packet to all clients
  net.Broadcast()
  ```

### Receive
  Register a callback function to handle a received packet with a specified name.
  
  #### Prototype  
  ```lua
  net.Receive("packetName", callback)
  ```
  
  #### Arguments  
  - `packetName` (string): The name of the packet to listen for.  
  - `callback` (function): The function to be called when the packet is received. This function will receive two parameters:
    - `len` (number): The length of the received data.  
    - `ply` (Player): The player who sent the packet.  
  
  #### Example
  ```lua
  -- Registering a callback to handle a "PlayerData" packet
  net.Receive("PlayerData", function(len, ply)
      print("Received data from player: " .. ply:GetName())
      -- Handle packet data
  end)
  ```
  ```lua
  -- Registering a callback to handle an Enemy destroyed
  net.Receive("EnemyDestroyed", function(len, ply)
    local enemyId = net.ReadInt()
    local dead = net.ReadBool()
    destroyEnemy(enemyId)
  end)
  ```

### Call
  Trigger the callback registered for a specific packet name, passing the packet length and player information.
  
  #### Prototype  
  ```lua
  net.Call("packetName", length, player)
  ```
  
  #### Arguments  
  `string`: The name of the packet to trigger the callback for.  
  `number`: The length of the packet data.  
  `ply` (Player): The player who is calling the packet.  
  
  #### Example  
  ```lua
  -- Triggering the "PlayerData" packet callback
  net.Call("PlayerData", 128, player)  -- '128' could be the length, 'player' is the player calling the packet
  ```

## Read & Write operations
__All operations here must have a read and write pair__
The network packet works like a stack, where you write data to the top of the stack and read it from the top of the stack. This means that the order in which you write and read data is important. If you write an integer, a float, and a string in that order, you must read them in the same order to get the correct values.

### WriteInt
  Check if an integer value can be written to the network packet.

  #### Prototype
  ```lua
  WriteInt(value: number) -> void
  ```

  #### Arguments
  `number`: The integer value to write into the network packet.

  #### Example
  ```lua
  WriteInt(42)
  ```

---

### ReadInt
  Retrieve an integer value from the network packet.

  #### Prototype
  ```lua
  ReadInt() -> number
  ```

  #### Arguments
  `None`

  #### Example
  ```lua
  local value = ReadInt()
  print("Received integer:", value)
  ```

---

### WriteFloat
  Write a floating-point value to the network packet.

  #### Prototype
  ```lua
  WriteFloat(value: number) -> void
  ```

  #### Arguments
  `number`: The floating-point value to write into the network packet.

  #### Example
  ```lua
  WriteFloat(3.14)
  ```

---

### ReadFloat
  Retrieve a floating-point value from the network packet.

  #### Prototype
  ```lua
  ReadFloat() -> number
  ```

  #### Arguments
  `None`

  #### Example
  ```lua
  local value = ReadFloat()
  print("Received float:", value)
  ```

---

### WriteString
  Write a string value to the network packet.

  #### Prototype
  ```lua
  WriteString(value: string) -> void
  ```

  #### Arguments
  `string`: The string to write into the network packet.

  #### Example
  ```lua
  WriteString("Hello, network!")
  ```

---

### ReadString
  Retrieve a string value from the network packet.

  #### Prototype
  ```lua
  ReadString() -> string
  ```

  #### Arguments
  `None`

  #### Example
  ```lua
  local value = ReadString()
  print("Received string:", value)
  ```

---

### WriteBool
  Write a boolean value to the network packet.

  #### Prototype
  ```lua
  WriteBool(value: boolean) -> void
  ```

  #### Arguments
  `boolean`: The boolean value to write into the network packet.

  #### Example
  ```lua
  WriteBool(true)
  ```

---

### ReadBool
  Retrieve a boolean value from the network packet.

  #### Prototype
  ```lua
  ReadBool() -> boolean
  ```

  #### Arguments
  `None`

  #### Example
  ```lua
  local value = ReadBool()
  print("Received boolean:", value)
  ```
