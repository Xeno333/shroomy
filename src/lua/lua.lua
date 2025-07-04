local sep = "  "

function dump(data, depth)
    local output = ""
    if data == nil then return output end

    -- Indentation
    local t_depth = depth or 0
    local tab = ""
    for i = 1, t_depth do
        tab = tab .. sep
    end

    -- Parse
    if type(data) == "table" then
        output = "{\n"

        for k, v in pairs(data) do
            output = output .. tab .. sep .. "[" .. dump(k, t_depth + 1) .. "] = " .. dump(v, t_depth + 1) .. "\n"
        end

        output = output .. tab .. "}"

    elseif type(data) == "string" then
        output = "\"" .. data .. "\""

    else
        output = tostring(data)
    end

    return output
end



function shroomy.random(min, max)
    if min < 0 then return nil end
    return math.floor(
        ((math.abs(shroomy.k_random())/18446744073709551615) * ((max * 2) - min + 1)) + min
    )
end