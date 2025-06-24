
entity = {}

function entity.new(tiles, pos, size)
    return {
        pos = {
            x = pos.x,
            y = pos.y
        },
        size = {
            x = size.x,
            y = size.y
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

            shroomy.render_texture(self.tiles[self.current_tile], self.pos.x, self.pos.y, self.size.x, self.size.y)
        end,
        is_collided = function(self, obj)
            local pos = self.pos
            local size = self.size
            local opos = obj.pos
            local osize = obj.size

            if (pos.y+(size.y) >= opos.y and pos.y <= opos.y+(osize.y)) and (pos.x+(size.x) >= opos.x and pos.x <= opos.x+(osize.x)) then
                return true
            end
            return false
        end
    }
end