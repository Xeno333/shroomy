#include <SDL2/SDL.h>
#include <iostream>
#include "interface.h"

void Interface::Render() {
    SDL_RenderPresent(Renderer);
}

void Interface::Clear() {
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
}

bool Interface::RenderTexture(std::string name, SDL_Rect* pos) {
    if (Textures.contains(name)) {
        SDL_RenderCopy(Renderer, Textures[name], NULL, pos);
        return true;
    }

    std::cout << "Texture `" << name << "` was not found!" << std::endl;

    return false;
}