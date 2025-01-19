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
  #### Prototype
  ```lua
  ```
  #### Arguments
  #### Example
  ```lua
  ```

### Call
  #### Prototype
  ```lua
  ```
  #### Arguments
  #### Example
  ```lua
  ```


## Read & Write operations
__All operations here must have a read and write pair__

### WriteInt
  #### Prototype
  ```lua
  ```
  #### Arguments
  #### Example
  ```lua
  ```

### ReadInt
  #### Prototype
  ```lua
  ```
  #### Arguments
  #### Example
  ```lua
  ```

### WriteFloat
  #### Prototype
  ```lua
  ```
  #### Arguments
  #### Example
  ```lua
  ```

### ReadFloat
  #### Prototype
  ```lua
  ```
  #### Arguments
  #### Example
  ```lua
  ```

### WriteString
  #### Prototype
  ```lua
  ```
  #### Arguments
  #### Example
  ```lua
  ```

### ReadString
  #### Prototype
  ```lua
  ```
  #### Arguments
  #### Example
  ```lua
  ```

### WriteBool
  #### Prototype
  ```lua
  ```
  #### Arguments
  #### Example
  ```lua
  ```

### ReadBool
  #### Prototype
  ```lua
  ```
  #### Arguments
  #### Example
  ```lua
  ```
