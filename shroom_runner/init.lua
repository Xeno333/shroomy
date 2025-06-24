local time = 0
local alive = false
local level = 0

local shroom_world = {
    x = 1200,
    y = 400
}
shroomy.set_window_size(shroom_world.x, shroom_world.y + 64)
shroomy.load_texture("shroom_boy", "textures/shroom_boy.png")
shroomy.load_texture("bad_shroom", "textures/bad_shroom.png")
shroomy.set_tick(17)



local shrooms = {}

local shroom_boy = entity.new({"shroom_boy", "bad_shroom"}, 100, {x = 100, y = shroom_world.y}, {y = 64, x = 64})

local jumping = 0
local hight = 0
local next_level_at = 0

function OnGameTick(time_ms)
    time = time + time_ms

    if shroomy.is_key_pressed("UP") then
        alive = true
        shrooms = {}
        hight = 0
        shroom_boy.pos = {x = 100, y = shroom_world.y}
        level = 0
        time = 0
        next_level_at = 0
        return
    end

    if alive then
        if time > next_level_at then
            next_level_at = time + 30000
            level = level + 1
        end
        if math.random(1,50) == 1 then
            shrooms[time] = entity.new({"bad_shroom"}, nil, {x = shroom_world.x-32, y = shroom_world.y+32}, {y = 32, x = 32})
        end

        local pos = shroom_boy.pos

        if pos.y >= shroom_world.y and shroomy.is_key_pressed("SPACE") then
            jumping = 200
        end

        local forward = shroomy.is_key_pressed("FORWARD")
        local backward = shroomy.is_key_pressed("BACKWARD")

        if pos.x <= shroom_world.x - 64 and forward and not backward then
            pos.x = pos.x + 10
        elseif pos.x >= 32 and backward and not forward then
            pos.x = pos.x - 10
        end

        -- Jump and fall
        if jumping > 0 then
            pos.y = pos.y - 10
            jumping = jumping - 10
            hight = hight + 10

        elseif pos.y < shroom_world.y then
            pos.y = pos.y + 7
            hight = hight - 7
        end

        shroom_boy:next_frame(time_ms)

        -- Bad shrooms
        for i, v in pairs(shrooms) do
            if shroom_boy:is_collided(v) then
                alive = false
                break
            end

            shrooms[i].pos.x = v.pos.x - 5
            if shrooms[i].pos.x < 0 then
                shrooms[i] = nil
            end
        end
    end
end



function RenderLoop()
    if alive then
        for i = 1, level do
            shroomy.render_texture("shroom_boy", 32 * i, 32, 32, 32)
        end

        shroom_boy:render()

        -- Bad shrooms
        for i, x in pairs(shrooms) do
            x:render()
        end

    else
        shroomy.render_texture("shroom_boy", 0, 0, shroom_world.x, shroom_world.y)
    end
end
