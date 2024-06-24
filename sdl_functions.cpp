#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <atomic>
#include <vector>
#include <algorithm>

#include "sdl_functions.h"

void closeButtonSetter(SDL_Renderer* ren, SDL_Rect closeButton, TTF_Font* font){
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

void handleTextInput(SDL_Event& event, std::string& inputText, size_t& cursorPosition) {
    if (event.type == SDL_TEXTINPUT) {
        inputText.insert(cursorPosition, event.text.text);
        cursorPosition += strlen(event.text.text);
    } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && cursorPosition > 0) {
            inputText.erase(cursorPosition - 1, 1);
            cursorPosition--;
        } else if (event.key.keysym.sym == SDLK_LEFT && cursorPosition > 0) {
            cursorPosition--;
        } else if (event.key.keysym.sym == SDLK_RIGHT && cursorPosition < inputText.length()) {
            cursorPosition++;
        }
    }
}

void renderTextInput(SDL_Renderer* renderer, TTF_Font* font, std::string& text, int x, int y,
                     bool cursorVisible, size_t& cursorPosition) {
    SDL_Color color = {0, 0, 0, 255};
    int textWidth = 0, textHeight = 0;
    int maxWidth = 250;

    TTF_SizeText(font, text.c_str(), &textWidth, &textHeight);
    while (textWidth > maxWidth && !text.empty()) {
        text.pop_back();
        TTF_SizeText(font, text.c_str(), &textWidth, &textHeight);
    }

    if (cursorPosition > text.length()) {
        cursorPosition = text.length();
    }

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
    
    if (cursorVisible) {
        int cursorX = x;
        if (cursorPosition <= text.size()) {
            std::string beforeCursor = text.substr(0, cursorPosition);
            TTF_SizeText(font, beforeCursor.c_str(), &cursorX, nullptr);
            cursorX += x;
        } else {
            cursorX += textWidth;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer, cursorX, y, cursorX, y + textHeight);
    }
}

bool insideRect(SDL_Rect rect, int mouseX, int mouseY){
    return (mouseX >= rect.x && mouseX <= (rect.x + rect.w) &&
        mouseY >= rect.y && mouseY <= (rect.y + rect.h));
}

bool insideImage(IconStruct image, int mouseX, int mouseY){
    return (mouseX >= image.x && mouseX <= (image.x + image.w) &&
            mouseY >= image.y && mouseY <= (image.y + image.h));
}

void renderCheckbox(SDL_Renderer* renderer, bool checked, int x, int y, int w, int h) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 150);
    SDL_RenderDrawRect(renderer, &rect);

    if (checked) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect innerRect = { x + 2, y + 2, w - 4, h - 4 };
        SDL_RenderFillRect(renderer, &innerRect);
    }
}

void sortItemLists(std::vector<std::string> &inputTextList, std::vector<bool> &selectedList) {
    std::vector<std::pair<std::string, bool>> combinedList;
    for (size_t i = 0; i < inputTextList.size(); ++i) {
        combinedList.emplace_back(inputTextList[i], selectedList[i]);
    }

    std::sort(combinedList.begin(), combinedList.end(),
              [](const std::pair<std::string, bool>& a, const std::pair<std::string, bool>& b) {
                  if (a.second != b.second) {
                      return a.second > b.second;
                  } else {
                      return a.first < b.first;
                  }
              });

    for (size_t i = 0; i < combinedList.size(); ++i) {
        inputTextList[i] = combinedList[i].first;
        selectedList[i] = combinedList[i].second;
    }
}
