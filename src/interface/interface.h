#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <map>
#include "shroomy.h"


#ifndef INTERFACE


class Interface {
    private:
        std::map<std::string, SDL_Texture*> Textures;
        std::map<std::string, Mix_Chunk*> Sounds;
        SDL_Window* Window = nullptr;
        SDL_Renderer* Renderer = nullptr;
        bool Valid = false;

        int background_red = 0;
        int background_green = 0;
        int background_blue = 0;

        const bool* KeyStates = nullptr;
        int KeyStatesLen = 0;

    public:
        bool IsValid();

        void GetEvents();
        bool IsKeyPressed(SDL_Scancode scancode);
        bool LoadTexture(std::string name, std::string path);
        bool LoadWav(std::string name, std::string path);
        bool PlaySound(std::string name, int loop);
        bool RenderTexture(std::string name, SDL_Rect* pos);
        void ResizeWindow(int w, int h);
        void SetWindowName(const std::string name);
        void SetWindowColor(int r, int g, int b);
        void Render();
        void Clear();

        Interface(bool print_inited) {
            if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
                std::cout << SDL_GetError() << std::endl;
                return;
            }
            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
                std::cout << Mix_GetError() << std::endl;
                return;
            }
            if (IMG_Init(IMG_INIT_PNG) == 0) {
                std::cout << IMG_GetError() << std::endl;
                return;
            }

            Window = SDL_CreateWindow(Shroomy::Version.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, 0);
            if (Window == nullptr) {
                std::cout << "SDL Could not create a window, SDL Error: " << SDL_GetError() << std::endl;
                return;
            }

            Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
            if (Window == nullptr) {
                std::cout << "SDL Could not create a window, SDL Error: " << SDL_GetError() << std::endl;
                return;
            }

            SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
            SDL_RenderClear(Renderer);
            SDL_RenderPresent(Renderer);

            if (print_inited)
                std::cout << "Video Initilized" << std::endl;

            Valid = true;
        };
        ~Interface() {
            if (Renderer != nullptr)
                SDL_DestroyRenderer(Renderer);

            if (Window != nullptr)
                SDL_DestroyWindow(Window);

            for (const auto &pair : Textures) {
                SDL_DestroyTexture(pair.second);
            }

            for (const auto &pair : Sounds) {
                Mix_FreeChunk(pair.second);
            }

            IMG_Quit();
            Mix_CloseAudio();
            SDL_Quit();
        };
};


#define INTERFACE

#endif