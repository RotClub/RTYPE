-- This function prints a table in a readable format, recursively.
function PrintTable(tbl: table, indents: number)
    indents = indents or 2
    local indent = string.rep("  ", indents)
    for k, v in pairs(tbl) do
        if type(v) == "table" then
            info(indent .. k .. ":")
            PrintTable(v, indents + 1)
        else
            info(indent .. k .. ": " .. tostring(v))
        end
    end
end
