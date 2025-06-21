#include "interface.h"



bool Interface::LoadTexture(std::string name, std::string path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr) {
        std::cout << "Texture '" << name << "' was not found at '" << path << "'!" << std::endl;
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == nullptr) {
        std::cout << "Texture '" << name << "' was not found!" << std::endl;
        return false;
    }

    Textures[name] = texture;
    return true;
}
