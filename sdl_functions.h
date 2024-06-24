#ifndef HAPPYWEATHER_SDL_FUNCTIONS_H
#define HAPPYWEATHER_SDL_FUNCTIONS_H

#include <vector>

struct IconStruct {
    int x;
    int y;
    int w;
    int h;
    IconStruct(int _x, int _y, int _w, int _h)
            : x(_x), y(_y), w(_w), h(_h) {}
};

void closeButtonSetter(SDL_Renderer* ren, SDL_Rect closeButton, TTF_Font* font);

int addIcon(SDL_Renderer* ren, const std::string& iconPath, int x, int y, int w, int h);

int addText(SDL_Renderer* ren, TTF_Font* font, const std::string& text, int x, int y);

void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Rect rect);

void handleTextInput(SDL_Event& event, std::string& inputText, size_t& cursorPosition);

void renderTextInput(SDL_Renderer* renderer, TTF_Font* font, std::string& text,
                     int x, int y,  bool cursorVisible, size_t& cursorPosition);

bool insideRect(SDL_Rect rect, int mouseX, int mouseY);

bool insideImage(IconStruct image, int mouseX, int mouseY);

void renderCheckbox(SDL_Renderer* renderer, bool checked, int x, int y, int w, int h);

void sortItemLists(std::vector<std::string> &inputTextList, std::vector<bool> &selectedList);



#endif //HAPPYWEATHER_SDL_FUNCTIONS_H
