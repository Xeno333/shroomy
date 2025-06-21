#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <map>
#include "../shroomy.h"


#ifndef INTERFACE


class Interface {
    private:
        std::map<std::string, SDL_Texture*> Textures;
        SDL_Window* Window = nullptr;
        SDL_Renderer* Renderer = nullptr;
        bool Valid = false;

        const bool* KeyStates = nullptr;
        int KeyStatesLen = 0;

    public:
        bool IsValid();
        void Close();

        void GetEvents();
        bool IsKeyPressed(SDL_Scancode scancode);
        bool LoadTexture(std::string name, std::string path);
        bool RenderTexture(std::string name, SDL_Rect* pos);
        void ResizeWindow(int w, int h);
        void Render();
        void Clear();

        Interface(bool print_inited) {
            if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
                std::cout << SDL_GetError() << std::endl;
                return;
            }
            if (IMG_Init(IMG_INIT_PNG) == 0) {
                std::cout << IMG_GetError() << std::endl;
                SDL_Quit();
                return;
            }

            Window = SDL_CreateWindow(Shroomy::Version.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, 0);
            if (Window == nullptr) {
                Close();
                std::cout << "SDL Could not create a window, SDL Error: " << SDL_GetError() << std::endl;
                return;
            }

            Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
            if (Window == nullptr) {
                Close();
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
            Close();
        };
};


#define INTERFACE true

#endif