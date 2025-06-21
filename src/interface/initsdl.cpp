#include <SDL2/SDL.h>
#include "interface.h"




bool Interface::IsValid() {
    if (Valid)
        return true;

    return false;
}

void Interface::ResizeWindow(int w, int h) {
    SDL_SetWindowSize(Window, w, h);
}