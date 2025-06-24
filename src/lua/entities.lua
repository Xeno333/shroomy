
entity = {}

function entity.new(tiles, frame_time_ms, pos, size)
    return {
        pos = {
            x = pos.x,
            y = pos.y
        },
        size = {
            x = size.x,
            y = size.y
        },
        frame_time_ms = frame_time_ms,
        tiles = tiles,

        -- Inital state
        last_frame_time_ms = 0,
        current_tile = 1,
        frame = tiles[1],

        -- Methods
        next_frame = function(self, time_since_last_ms)
            local change_frame = false

            -- Time frames
            if time_since_last_ms then
                self.last_frame_time_ms = self.last_frame_time_ms + time_since_last_ms
                if self.last_frame_time_ms >= self.frame_time_ms then
                    change_frame = true
                    self.last_frame_time_ms = 0
                end
            end

            if change_frame and #(self.tiles) ~= 1 then
                if #(self.tiles) == self.current_tile then
                    self.current_tile = 1
                else
                    self.current_tile = self.current_tile + 1
                end

                self.frame = self.tiles[self.current_tile]
            end

        end,
        render = function(self)
            shroomy.render_texture(self.frame, self.pos.x, self.pos.y, self.size.x, self.size.y)
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