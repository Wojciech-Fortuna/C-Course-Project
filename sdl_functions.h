//
// Created by Wojciech Fortuna on 20.06.2024.
//

#ifndef HAPPYWEATHER_SDL_FUNCTIONS_H
#define HAPPYWEATHER_SDL_FUNCTIONS_H


void closeButtonSetter(SDL_Renderer* ren, SDL_Rect closeButton, TTF_Font* font);

int addIcon(SDL_Renderer* ren, const std::string& iconPath, int x, int y, int w, int h);

int addText(SDL_Renderer* ren, TTF_Font* font, const std::string& text, int x, int y);

void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Rect rect);

void handleTextInput(SDL_Event& event, std::string& inputText);

void renderTextInput(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y);

struct IconStruct {
    int x;
    int y;
    int w;
    int h;
    IconStruct(int _x, int _y, int _w, int _h)
            : x(_x), y(_y), w(_w), h(_h) {}
};

#endif //HAPPYWEATHER_SDL_FUNCTIONS_H
