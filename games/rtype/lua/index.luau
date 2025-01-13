if SERVER then
    include("server/init.luau")
    include("metatable_test.luau")
elseif CLIENT then
    include("client/init.luau")
end
