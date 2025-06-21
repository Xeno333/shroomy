
local shroom_world = {
    x = 1200,
    y = 400
}
shroomy.set_window_size(shroom_world.x, shroom_world.y + 64)
shroomy.load_texture("shroom_boy", "textures/shroom_boy.png")
shroomy.load_texture("bad_shroom", "textures/bad_shroom.png")




local shooms = {}

function OnGameTick()
    --shroomy.shroomy_say("Tick!")
    if math.random(1, 10) == 1 then
        shooms[#shooms + 1] = shroom_world.x
    end
end


local pos = {
    x = 100,
    y = shroom_world.y,
}
local jumping = 0
local hight = 0

local alive = true

function GameLoop()
    shroomy.render_texture("shroom_boy", 0, 0, shroom_world.x, shroom_world.y)

    if alive then
        if pos.y >= shroom_world.y and shroomy.is_key_pressed("SPACE") then
            jumping = 200
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

        shroomy.render_texture("shroom_boy", pos.x, pos.y, 64, 64)
    end

    for i, x in pairs(shooms) do
        if not (hight > 32) and ((pos.x > x and pos.x < x+32) or (pos.x < x and pos.x > x-32)) then
            alive = false
        end
        shroomy.render_texture("bad_shroom", x, shroom_world.y+32, 32, 32)

        if alive then
            shooms[i] = x - 5
        end
    end

end
