# Lua API table

All the lua API functions are located in the `shroomy` table.

- `shroomy_say(msg)`
    Prints message for demo.

- `load_wav(name, path)`
    Load a `.wav` file at `path` into engine sounds cache with the ID of `name`, where `name` is a string. Returns `nil` on bad params, otherwise boolean.
    
- `play_sound(name)`
    Play sound with ID of `name`.Returns `nil` if `name` not defined. Returns `nil` on bad params, otherwise boolean.

- `load_texture(name, path)`
    Loads `.png` texture at `path` into engine textures cache with ID of `name`, where `name` is a string. Returns `nil` on bad params, otherwise boolean.

- `render_texture(name, x, y, w, h)`
    Renders texture of ID `name`,  where `name` is a string, at position `(x, y)` at a resolution of `(w, h)`. Returns `nil` on bad params, otherwise boolean.
    **NOTE**: A more standard form of this is planned.

- `is_key_pressed(key_id)`
    Returns true or false for state of scan code mapped to `key_id` by game conf, returns `nil` if not defined.

- `set_window_size(w, h)`
    Set window size in pixels of `w, h`. Returens `true` unless bad params, in which case `false` is returned.

- `set_tick(ms)`
    Set tick rate at which `OnGameTick(ms)` is called. Returns `true`/`false` based on params being valid number. Default is `10` ms.

- `random(min, max)`
    Returns an integar between min and max. **WARNING**: Not true random.

- `k_random()`
    Returns an number. **WARNING**: Not true random, this is for internal use in `random`.

# Lua provided

This type of function is a global function that can be called for anything.

- `dump(data)`
    Dumps data of any type, including tables.


# Entities through the lua `entity` API

There is a entity API defined internaly in lua, i.e. it runs in lua and does all the C++ calls that are needed to provide a simple interface. Entities use a class like structure anc can be created using `entity.new(tiles, frame_time_ms, pos, size)`.

## `entity` API

- `new(tiles, frame_time_ms, pos, size)`
    Returns an entity object.

## Entity object

Entities are objects defined in lua. The methods are all shared with `entity`.

### Methods

- `next_frame(self, time_since_last_ms)`
    Loads next frame for rendering, does not render. `time_since_last_ms` is the number of ms since last frame switch, to ensure proper FPS for an entity.

- `render(self)`
    Render entity.

- `is_collided(self, collider)`
    Check for collision between `self` and `collider`, where both are entities.

### Data

- `pos = {x = <x>, y = <y>}`
    Position of entity.


# Game defined functions

In order for a game to run one of the two game functions **MUST** be set.

- `RenderLoop()`
    Run every frame, this is for rendering, high res physics, etc.

- `OnGameTick(ms)`
    Run once every game tick. For main game code, controls etc.