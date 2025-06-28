local time = 0
local shroom_world = {
    x = 1200,
    y = 400
}

local clients = {}


if shroomy.mode == "client" then
    shroomy.set_tick(17)

    shroomy.set_window_size(shroom_world.x, shroom_world.y + 64)
    shroomy.load_texture("shroom_boy", "textures/shroom_boy.png")
    shroomy.set_window_name("shrooms multiplayer")

    function RenderLoop()
        for _, v in pairs(clients) do
            v:render()
        end
    end

    shroomy.send_to_server({1})

    local client_id = nil
    local last = 0

    function Client(data)
        if data then
            if data[1] ~= 12345678 then return end

            if not client_id then
                client_id = data[2]
            end

            if not clients[data[3]] then
                clients[data[3]] = entity.new({"shroom_boy"}, 0, {x = 100, y = shroom_world.y}, {y = 64, x = 64}, {x_min = -24, x_max = 24, y_min = -24, y_max = 32})
            end

            clients[data[3]].pos.x = data[4]
            clients[data[3]].pos.y = data[5]
        end
    end

    function OnGameTick(time_ms)
        time = time + time_ms
        if client_id == nil then
            return
        end
        local me = clients[client_id]

        if shroomy.is_key_pressed("UP") then
            shroomy.send_to_server({12345678, 1, 0})
            me.pos.y = math.max(0, me.pos.y - 5)
        elseif shroomy.is_key_pressed("DOWN") then
            shroomy.send_to_server({12345678, 1, 1})
            me.pos.y = math.max(0, me.pos.y + 5)
        elseif shroomy.is_key_pressed("BACKWARD") then
            shroomy.send_to_server({12345678, 1, 2})
            me.pos.x = math.max(0, me.pos.x - 5)
        elseif shroomy.is_key_pressed("FORWARD") then
            shroomy.send_to_server({12345678, 1, 3})
            me.pos.x = math.max(0, me.pos.x + 5)
        end
    end

elseif shroomy.mode == "server" then
    shroomy.set_tick(17/2)

    function Server(id, data)
        if not data then
            clients[id] = nil
            return

        elseif not clients[id] then
            clients[id] = entity.new({"shroom_boy"}, 0, {x = 100, y = shroom_world.y}, {y = 64, x = 64}, {x_min = -24, x_max = 24, y_min = -24, y_max = 32})
            return
        end

        if data[1] == 12345678 and data[2] == 1 then
            if data[3] == 0 then
                clients[id].pos.y = math.max(0, clients[id].pos.y - 5)
            elseif data[3] == 1 then
                clients[id].pos.y = math.max(0, clients[id].pos.y + 5)
            elseif data[3] == 2 then
                clients[id].pos.x = math.max(0, clients[id].pos.x - 5)
            elseif data[3] == 3 then
                clients[id].pos.x = math.max(0, clients[id].pos.x + 5)
            end
        end
    end

    function OnGameTick(time_ms)
        time = time + time_ms

        for i, _ in pairs(clients) do
            for k, v in pairs(clients) do
                shroomy.send_to_client(i, {12345678, i, k, v.pos.x, v.pos.y})
            end
        end
    end

else
    print("Only Server or Client modes supported!")
end