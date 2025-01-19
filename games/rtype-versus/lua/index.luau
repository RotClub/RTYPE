include("common/init.luau")
if SERVER then
    include("server/init.luau")
elseif CLIENT then
    include("client/init.luau")
end
