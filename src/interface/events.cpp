
#include <SDL2/SDL.h>
#include "interface.h"
#include "../main.h"


void Interface::GetEvents() {
    SDL_PumpEvents();
    KeyStates = (const bool*)SDL_GetKeyboardState(&KeyStatesLen);

    SDL_Event sdl_event;
    SDL_PollEvent(&sdl_event);
    if (sdl_event.type == SDL_QUIT)
        Main::Running = false;
}


bool Interface::IsKeyPressed(SDL_Scancode scancode) {
    if (KeyStates == nullptr || KeyStatesLen == 0)
        return false;

    if (KeyStatesLen >= scancode) {
        if (KeyStates[scancode]) 
            return true;
    }

    return false;
}