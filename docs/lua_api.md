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