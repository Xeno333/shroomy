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

void Interface::Close() {
    if (Renderer != nullptr)
        SDL_DestroyRenderer(Renderer);

    if (Window != nullptr)
        SDL_DestroyWindow(Window);

    for (const auto &pair : Textures) {
        SDL_DestroyTexture(pair.second);
    }

    SDL_Quit();
    IMG_Quit();
}