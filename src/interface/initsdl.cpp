#include <SDL2/SDL.h>
#include "interface.h"
#include "shroomy.h"




bool Interface::IsValid() {
    if (Valid)
        return true;

    return false;
}

void Interface::ResizeWindow(int w, int h) {
    if (Valid)
        SDL_SetWindowSize(Window, w, h);
}

void Interface::SetWindowName(const std::string name) {
    if (Valid)
        SDL_SetWindowTitle(Window, (Shroomy::Version + " - " + name).c_str());
}