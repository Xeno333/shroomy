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




entity = {}

function entity.new(tiles, pos)
    return {
        pos = {
            x = pos.x,
            y = pos.y
        },
        tiles = tiles,
        current_tile = 1,
        render_next_frame = function(self)
            if #(self.tiles) ~= 1 then
                if #(self.tiles) == self.current_tile then
                    self.current_tile = 1
                else
                    self.current_tile = self.current_tile + 1
                end
            end

            local tile = self.tiles[self.current_tile]

            shroomy.render_texture(tile.texture, self.pos.x, self.pos.y, tile.w, tile.h)
        end
    }
end