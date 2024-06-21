#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "sdl_functions.h"

void createSettingsWindow(SDL_Window* mainWindow) {
    SDL_Window* settingsWin = SDL_CreateWindow("Settings", 200, 200, 640, 480, SDL_WINDOW_SHOWN);
    if (settingsWin == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Renderer* settingsRen = SDL_CreateRenderer(settingsWin, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (settingsRen == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(settingsWin);
        return;
    }

    std::string fontPath = "C:/Users/Wojciech Fortuna/Cpp_Project/Roboto/Roboto-Black.ttf";
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), 12);
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(settingsRen);
        SDL_DestroyWindow(settingsWin);
        SDL_Quit();
        return;
    }

    bool settingsQuit = false;
    SDL_Rect closeButton = {0, 430, 100, 50};
    closeButtonSetter(settingsRen, closeButton, font);

    SDL_Event settingsEvent;
    while (!settingsQuit) {
        while (SDL_PollEvent(&settingsEvent)) {
            if (settingsEvent.type == SDL_QUIT) {
                settingsQuit = true;
                std::cout << "Settings window closed." << std::endl;
                break;
            }
            else if (settingsEvent.type == SDL_MOUSEBUTTONDOWN) {
                if (settingsEvent.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = settingsEvent.button.x;
                    int mouseY = settingsEvent.button.y;
                    if (mouseX >= closeButton.x && mouseX <= (closeButton.x + closeButton.w) &&
                        mouseY >= closeButton.y && mouseY <= (closeButton.y + closeButton.h)) {
                        settingsQuit = true;
                        break;
                    }
                }
            }
        }


    }

    SDL_DestroyRenderer(settingsRen);
    SDL_DestroyWindow(settingsWin);
}


