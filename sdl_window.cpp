#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <chrono>

std::atomic<bool> quit(false);
std::condition_variable dataReady;
int bad_days;
std::vector<std::string> iconPathList;
std::vector<std::string> temperatureList;
std::vector<std::string> descriptionList;
std::vector<std::string> mainDescriptionList;
std::vector<std::string> timeList;
int forecastDays = 5;
std::string location = "Cracow";
bool immediateChange = false;

#include "sdl_window.h"
#include "weatherData.h"
#include "settingsWindow.h"
#include "sdl_functions.h"
#include "nr_days.h"



void updateWeatherData() {


    while (!quit) {
        {
            getCurrentData(iconPathList, temperatureList,
                          descriptionList, mainDescriptionList, timeList, forecastDays, location);
            for (int period = 0; period < 1500; period++){
                if (quit || immediateChange){
                    immediateChange = true;
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        }
    }
}


int createMainWindow(const std::vector<std::string>& iconPathList_loc,
                     const std::vector<std::string>& temperatureList_loc,
                     const std::vector<std::string>& descriptionList_loc,
                     const std::vector<std::string>& mainDescriptionList_loc,
                     const std::vector<std::string>& timeList_loc) {

    iconPathList = iconPathList_loc;
    temperatureList = temperatureList_loc;
    descriptionList = descriptionList_loc;
    mainDescriptionList = mainDescriptionList_loc;
    timeList = timeList_loc;


    std::vector<std::string> inputTextList;
    std::string inputText;
    bool isTextInputActive = false;
    bool cursorVisible = true;
    bool cursorActive = false;
    Uint32 lastBlinkTime = 0;
    int CURSOR_BLINK_INTERVAL = 500;
    size_t cursorPosition = 0;
    std::vector<std::tuple<int, int>> checkboxCheckedList;
    std::vector<bool> selectedList;

    int scrollOffset = 0;
    int itemHeight = 150;
    int numItems = iconPathList.size();
    int totalHeight = numItems * itemHeight;

    int scrollOffset_text = 0;
    const int textHeight = 40;
    int numTexts = inputTextList.size();
    int totalHeight_text = numTexts * textHeight;
    IconStruct redCross(660, 0, 30, 30);
    IconStruct checkBox(710, 0, 30, 30);
    std::vector<std::tuple<int, int>> redCrossesPositions;

    bad_days = categorizeWeatherByDay(timeList, mainDescriptionList);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("HappyWeather", 100, 100, 1080, 640, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (win == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    addIcon(ren, "C:/Users/Wojciech Fortuna/Cpp_Project/settings-icon.png", 20, 20, 50, 50);
    IconStruct iconSettings(20, 20, 50, 50);


    std::string fontPath = "C:/Users/Wojciech Fortuna/Cpp_Project/Roboto/Roboto-Black.ttf";
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), 14);
    TTF_Font* font_big = TTF_OpenFont(fontPath.c_str(), 36);
    if (!font || !font_big) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    SDL_Rect scrollbar = {1060, 0, 20, 100};
    bool scrollbarDragging = false;
    int scrollbarOffset = 0;

    SDL_Rect scrollbar_text = {780, 300, 10, 40};
    bool scrollbarDragging_text = false;
    int scrollbarOffset_text = 0;

    SDL_Rect whiteSpace = {390, 300, 420, 340};

    SDL_Rect listItemsButton = {390, 250, 420, 50};

    SDL_Rect closeButton = {0, 590, 100, 50};
    closeButtonSetter(ren, closeButton, font);

    SDL_Rect sdlBadDays = {200, 100, 600, 50};
    closeButtonSetter(ren, sdlBadDays, font);

    SDL_SetRenderDrawColor(ren, 0, 255, 255, 0);

    SDL_Rect newTextButton = {50, 200, 300, 50};

    SDL_Rect inputRect = {50, 250, 300, 50};

    SDL_Rect saveButton = {50, 300, 150, 50};

    SDL_Rect clearButton = {200, 300, 150, 50};


    bool another_e = false;
    std::thread weatherThread(updateWeatherData);

    SDL_StopTextInput();
    SDL_Event e;
    while (!quit) {
        numItems = iconPathList.size();
        totalHeight = numItems * itemHeight;
        numTexts = inputTextList.size();
        totalHeight_text = numTexts * textHeight;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                break;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    if (e.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;
                    if (insideImage(iconSettings, mouseX, mouseY)){
                        another_e = true;
                        break;
                    }
                    if (insideRect(closeButton, mouseX, mouseY)){
                        quit = true;
                        break;
                    }
                }
                if (e.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;
                    if (insideRect(scrollbar, mouseX, mouseY)){
                        scrollbarDragging = true;
                        scrollbarOffset = mouseY - scrollbar.y;
                    }
                    if (insideRect(scrollbar_text, mouseX, mouseY)){
                        scrollbarDragging_text = true;
                        scrollbarOffset_text = mouseY - scrollbar_text.y;
                    }
                    else if (insideRect(inputRect, mouseX, mouseY)){
                        SDL_StartTextInput();
                        isTextInputActive = true;
                        cursorActive = true;
                    }
                    else if (insideRect(saveButton, mouseX, mouseY)){
                        SDL_StopTextInput();
                        isTextInputActive = false;
                        inputTextList.push_back(inputText);
                        selectedList.push_back(false);
                        inputText.clear();
                        cursorPosition = 0;
                        cursorActive = false;
                    }
                    else if (insideRect(clearButton, mouseX, mouseY)){
                        SDL_StopTextInput();
                        isTextInputActive = false;
                        inputText.clear();
                        cursorPosition = 0;
                        cursorActive = false;
                    }
                    else {
                        SDL_StopTextInput();
                        isTextInputActive = false;
                        cursorActive = false;
                    }
                    for (const auto& tuple : redCrossesPositions) {
                        int crossPosition = std::get<0>(tuple);
                        if (mouseX >= redCross.x && mouseX <= redCross.x + redCross.w &&
                                mouseY >= crossPosition && mouseY <= crossPosition + redCross.h){
                            int deletePosition = std::get<1>(tuple);
                            inputTextList.erase(inputTextList.begin() + deletePosition);
                            selectedList.erase(selectedList.begin() + deletePosition);
                        }
                    }
                    for (auto& tuple : checkboxCheckedList) {
                        int checkBoxPosition = std::get<0>(tuple);
                        if (mouseX >= checkBox.x && mouseX <= checkBox.x + checkBox.w &&
                            mouseY >= checkBoxPosition && mouseY <= checkBoxPosition + checkBox.h) {
                            int checkBoxIndex = std::get<1>(tuple);
                            selectedList[checkBoxIndex] = !selectedList[checkBoxIndex];
                        }
                    }
                }
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    scrollbarDragging = false;
                    scrollbarDragging_text = false;
                }
            } else if (e.type == SDL_MOUSEMOTION) {
                if (scrollbarDragging) {
                    int mouseY = e.motion.y;
                    scrollbar.y = mouseY - scrollbarOffset;
                    if (scrollbar.y < 0) scrollbar.y = 0;
                    if (scrollbar.y > 640 - scrollbar.h) scrollbar.y = 640 - scrollbar.h;
                    scrollOffset = (scrollbar.y * totalHeight) / 610;

                }
                else if (scrollbarDragging_text) {
                    int mouseY_text = e.motion.y;
                    scrollbar_text.y = mouseY_text - scrollbarOffset_text;
                    if (scrollbar_text.y < 300) scrollbar_text.y = 300;
                    if (scrollbar_text.y > 640 - scrollbar_text.h) scrollbar_text.y = 640 - scrollbar_text.h;
                    scrollOffset_text = ((scrollbar_text.y - 300) * totalHeight_text) / 340;
                }
            } else if (e.type == SDL_TEXTINPUT || e.type == SDL_KEYDOWN) {
                if (isTextInputActive) {
                    handleTextInput(e, inputText, cursorPosition);
                }
            }
        }
        if (another_e){
            createSettingsWindow(e, forecastDays, location, immediateChange);
            another_e = false;
        }
        if (quit){
            break;
        }

        SDL_SetRenderDrawColor(ren, 0, 255, 255, 0);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderFillRect(ren, &whiteSpace);

        sortItemLists(inputTextList, selectedList);

        for (int i = 0; i < numItems; i++) {
            int yPosition = i * itemHeight - scrollOffset;
            if (yPosition >= 640 || yPosition + itemHeight < 0) continue;

            addIcon(ren, iconPathList[i], 820, yPosition + 10, 100, 100);
            addText(ren, font, temperatureList[i], 920, yPosition + 10);
            addText(ren, font, descriptionList[i], 920, yPosition + 40);
            addText(ren, font, timeList[i], 920, yPosition + 70);
        }
        redCrossesPositions.clear();
        checkboxCheckedList.clear();
        for (int i = 0; i < numTexts; i++) {
            int yPosition = i * textHeight - scrollOffset_text + 300;
            if (yPosition >= 640 || yPosition < 300) continue;
            addText(ren, font, inputTextList[i], 400, yPosition + 10);
            addIcon(ren, "C:/Users/Wojciech Fortuna/Cpp_Project/red_cross_image.png",
                    redCross.x, yPosition + 5, redCross.w, redCross.h);
            redCrossesPositions.emplace_back(yPosition + 5, i);


            renderCheckbox(ren, selectedList[i],
                    checkBox.x, yPosition + 5, checkBox.w, checkBox.h);
            checkboxCheckedList.emplace_back(yPosition + 5, i);
        }

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastBlinkTime >= CURSOR_BLINK_INTERVAL) {
            cursorVisible = !cursorVisible;
            lastBlinkTime = currentTime;
        }
        if (!cursorActive){
            cursorVisible = false;
        }

        addIcon(ren, "C:/Users/Wojciech Fortuna/Cpp_Project/settings-icon.png", 20, 20, 50, 50);

        SDL_SetRenderDrawColor(ren, 150, 150, 150, 255);
        SDL_RenderFillRect(ren, &scrollbar);

        if (!inputTextList.empty()){
            SDL_SetRenderDrawColor(ren, 150, 150, 150, 255);
            SDL_RenderFillRect(ren, &scrollbar_text);
        }

        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        SDL_RenderFillRect(ren, &closeButton);
        renderText(ren, font, "Close", closeButton);

        SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
        SDL_RenderFillRect(ren, &sdlBadDays);
        renderText(ren, font_big, "Prepare for " + std::to_string(bad_days) +
                                  " days with bad weather", sdlBadDays);

        SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
        SDL_RenderFillRect(ren, &newTextButton);
        renderText(ren, font, "Add new item to the list", newTextButton);

        SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
        SDL_RenderFillRect(ren, &listItemsButton);
        renderText(ren, font, "List of items for 1 day:", listItemsButton);

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderFillRect(ren, &inputRect);
        renderTextInput(ren, font, inputText, inputRect.x + 10, inputRect.y + 10,
                        cursorVisible, cursorPosition);

        SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
        SDL_RenderFillRect(ren, &saveButton);
        renderText(ren, font, "Save", saveButton);

        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        SDL_RenderFillRect(ren, &clearButton);
        renderText(ren, font, "Clear", clearButton);

        SDL_RenderPresent(ren);
    }


    weatherThread.join();

    SDL_RenderClear(ren);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}


