#include "interface.h"


bool Interface::LoadWav(std::string name, std::string path) {
    InterfaceWavSpec *spec = new InterfaceWavSpec(path);

    if (spec -> valid)
        Sounds[name] = spec;
    else {
        delete spec;
        return false;
    }

    return true;
}

bool Interface::PlayWav(std::string name) {
    if (Sounds.contains(name)) {
        if (SDL_QueueAudio(Sounds[name] -> AudioDevice, Sounds[name] -> Buffer, Sounds[name] -> Len) == 0) {
            SDL_PauseAudioDevice(Sounds[name] -> AudioDevice, 0);
            return true;
        }else {
            std::cout << SDL_GetError() << std::endl;
        }
    }

    return false;
}
