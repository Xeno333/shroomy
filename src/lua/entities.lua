
entity = {}


if shroomy.IS_DEBUG then
    function entity.render(self)
        shroomy.render_texture(self.frame, self.pos.x, self.pos.y, self.size.x, self.size.y)
        local x1 = self.pos.x + self.center_render_offset.x + self.collision_size.x_min
        local x2 = self.pos.x + self.center_render_offset.x + self.collision_size.x_max
        local y1 = self.pos.y + self.center_render_offset.y + self.collision_size.y_min
        local y2 = self.pos.y + self.center_render_offset.y + self.collision_size.y_max

        for y = self.pos.y + self.center_render_offset.y + self.collision_size.y_min, self.pos.y + self.center_render_offset.y + self.collision_size.y_max, 1 do
            shroomy.render_texture(self.frame, x1, y, 1, 1)
            shroomy.render_texture(self.frame, x2, y, 1, 1)
        end
        for x = self.pos.x + self.center_render_offset.x + self.collision_size.x_min, self.pos.x + self.center_render_offset.x + self.collision_size.x_max, 1 do
            shroomy.render_texture(self.frame, x, y1, 1, 1)
            shroomy.render_texture(self.frame, x, y2, 1, 1)
        end
    end
else
    function entity.render(self)
        shroomy.render_texture(self.frame, self.pos.x, self.pos.y, self.size.x, self.size.y)
    end
end


function entity.next_frame(self, time_since_last_ms)
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
end


function entity.is_collided(self, obj)
    local x_min = self.pos.x + self.center_render_offset.x + self.collision_size.x_min
    local x_max = self.pos.x + self.center_render_offset.x + self.collision_size.x_max
    local y_min = self.pos.y + self.center_render_offset.y + self.collision_size.y_min
    local y_max = self.pos.y + self.center_render_offset.y + self.collision_size.y_max

    local o_x_min = obj.pos.x + obj.center_render_offset.x + obj.collision_size.x_min
    local o_x_max = obj.pos.x + obj.center_render_offset.x + obj.collision_size.x_max
    local o_y_min = obj.pos.y + obj.center_render_offset.y + obj.collision_size.y_min
    local o_y_max = obj.pos.y + obj.center_render_offset.y + obj.collision_size.y_max

    if  (y_max >= o_y_min and y_min <= o_y_max) and
        (x_max >= o_x_min and x_min <= o_x_max) then

        return true
    end
    return false
end




function entity.new(tiles, frame_time_ms, pos, size, collision_size)
    return {
        pos = {
            x = pos.x,
            y = pos.y
        },

        -- Internal data
        size = {
            x = size.x,
            y = size.y
        },
        center_render_offset = {
            x = size.x / 2,
            y = size.y / 2
        },
        collision_size = {
            x_min = (collision_size or {}).x_min or -(size.x / 2),
            x_max = (collision_size or {}).x_max or (size.x / 2),
            y_min = (collision_size or {}).y_min or -(size.y / 2),
            y_max = (collision_size or {}).y_max or (size.y / 2),
        },
        frame_time_ms = frame_time_ms,
        tiles = tiles,

        -- Inital state
        last_frame_time_ms = 0,
        current_tile = 1,
        frame = tiles[1],

        -- Methods
        next_frame = entity.next_frame,
        render = entity.render,
        is_collided = entity.is_collided
    }
end