#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <atomic>

#include "sdl_functions.h"

void closeButtonSetter(SDL_Renderer* ren, SDL_Rect closeButton, TTF_Font* font){
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderClear(ren);

    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderFillRect(ren, &closeButton);

    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Close", textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ren, textSurface);
    SDL_Rect textRect = {closeButton.x + 30, closeButton.y + 15, textSurface->w, textSurface->h};
    SDL_RenderCopy(ren, textTexture, nullptr, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    SDL_RenderPresent(ren);
};

int addIcon(SDL_Renderer* ren, const std::string& iconPath, int x, int y, int w, int h) {
    SDL_Surface* iconSurface = IMG_Load(iconPath.c_str());
    if (!iconSurface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return 1;
    }

    SDL_Texture* iconTexture = SDL_CreateTextureFromSurface(ren, iconSurface);
    SDL_FreeSurface(iconSurface);
    if (!iconTexture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Rect iconDest = {x, y, w, h};
    SDL_RenderCopy(ren, iconTexture, NULL, &iconDest);
    SDL_DestroyTexture(iconTexture);

    return 0;
}

int addText(SDL_Renderer* ren, TTF_Font* font, const std::string& text, int x, int y) {
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (!textSurface) {
        std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ren, textSurface);
    if (!textTexture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return 1;
    }

    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(ren, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    return 0;
}


void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Rect rect) {
    SDL_Color color = { 0, 0, 0 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (surface == nullptr) {
        std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect textRect;
    textRect.x = rect.x + (rect.w - surface->w) / 2;
    textRect.y = rect.y + (rect.h - surface->h) / 2;
    textRect.w = surface->w;
    textRect.h = surface->h;

    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void handleTextInput(SDL_Event& event, std::string& inputText) {
    if (event.type == SDL_TEXTINPUT) {
        inputText += event.text.text;
    } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0) {
            inputText.pop_back();
        }
    }
}

void renderTextInput(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y) {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture) {
            SDL_Rect rect = {x, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, nullptr, &rect);
            SDL_DestroyTexture(texture);
        }
        SDL_FreeSurface(surface);
    }
}
