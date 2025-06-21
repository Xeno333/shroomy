#include "interface.h"


bool Interface::LoadWav(std::string name, std::string path) {
    Mix_Chunk *sound = Mix_LoadWAV(path.c_str());

    if (sound != nullptr)
        Sounds[name] = sound;
    else {
        std::cout << "Wav file '" << name << "' could not be loaded!" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

bool Interface::PlaySound(std::string name, int loop) {
    if (Sounds.contains(name)) {
        if (Mix_PlayChannel(-1, Sounds[name], loop) != -1) {
            return true;
        } else {
            std::cout << SDL_GetError() << std::endl;
        }
    } else {
        std::cout << "Sound '" << name << "' was not found!" << std::endl;
    }

    return false;
}
