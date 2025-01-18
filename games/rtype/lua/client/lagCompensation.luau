
function correctPosition(entity, x: number, y: number)
    local eX, eY = entity:GetGlobalPosition()
    entity:SetPosition((eX + x) / 2, (eY + y) / 2)
end
