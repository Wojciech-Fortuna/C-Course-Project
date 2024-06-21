#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <unordered_map>
#include <ctime>

std::atomic<bool> quit(false);
std::mutex weatherDataMutex;
int bad_days;

#include "sdl_window.h"
#include "weatherData.h"
#include "settingsWindow.h"
#include "sdl_functions.h"
#include "nr_days.h"




void updateWeatherData(std::vector<std::string> iconPathList, std::vector<std::string> temperatureList,
                       std::vector<std::string> descriptionList, std::vector<std::string> mainDescriptionList,
                       std::vector<std::string> timeList) {
    while (!quit) {
        for (int i = 0; i < 1500 && !quit; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        if (quit){
            break;
        }
        std::lock_guard<std::mutex> lock(weatherDataMutex);

        getCurrentData(iconPathList, temperatureList, descriptionList, mainDescriptionList, timeList);

        bad_days = categorizeWeatherByDay(timeList, mainDescriptionList);
        std::cout << "nr_days" << bad_days << std::endl;
    }
}


int createMainWindow(const std::vector<std::string>& iconPathList, const std::vector<std::string>& temperatureList,
                     const std::vector<std::string>& descriptionList,
                     const std::vector<std::string>& mainDescriptionList, const std::vector<std::string>& timeList) {

    bad_days = categorizeWeatherByDay(timeList, mainDescriptionList);
    std::cout << "nr_days" << bad_days << std::endl;


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
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), 12);
    TTF_Font* font_big = TTF_OpenFont(fontPath.c_str(), 36);
    if (!font || !font_big) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    int scrollOffset = 0;
    const int itemHeight = 150;
    const int numItems = iconPathList.size();
    const int totalHeight = numItems * itemHeight;

    SDL_Rect scrollbar = {1060, 0, 20, 100};
    bool scrollbarDragging = false;
    int scrollbarOffset = 0;

    SDL_Rect closeButton = {0, 590, 100, 50};
    closeButtonSetter(ren, closeButton, font);

    SDL_Rect sdlBadDays = {200, 100, 600, 50};
    closeButtonSetter(ren, sdlBadDays, font);

    SDL_SetRenderDrawColor(ren, 0, 255, 255, 0);

    SDL_Rect inputRect = {100, 100, 400, 50};


    std::thread weatherThread(updateWeatherData, std::ref(iconPathList), std::ref(temperatureList),
                              std::ref(descriptionList), std::ref(mainDescriptionList), std::ref(timeList));

    std::string inputText;
    bool saveClicked = false;

    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                break;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;
                    if (mouseX >= iconSettings.x && mouseX <= (iconSettings.x + iconSettings.w) &&
                        mouseY >= iconSettings.y && mouseY <= (iconSettings.y + iconSettings.h)) {
                        createSettingsWindow(win);
                    }
                    if (mouseX >= closeButton.x && mouseX <= (closeButton.x + closeButton.w) &&
                        mouseY >= closeButton.y && mouseY <= (closeButton.y + closeButton.h)) {
                        quit = true;
                        break;
                    }
                }
                if (e.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;
                    if (mouseX >= scrollbar.x && mouseX <= scrollbar.x + scrollbar.w &&
                        mouseY >= scrollbar.y && mouseY <= scrollbar.y + scrollbar.h) {
                        scrollbarDragging = true;
                        scrollbarOffset = mouseY - scrollbar.y;
                    }
                }
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    scrollbarDragging = false;
                }
            } else if (e.type == SDL_MOUSEMOTION) {
                if (scrollbarDragging) {
                    int mouseY = e.motion.y;
                    scrollbar.y = mouseY - scrollbarOffset;
                    if (scrollbar.y < 0) scrollbar.y = 0;
                    if (scrollbar.y > 640 - scrollbar.h) scrollbar.y = 640 - scrollbar.h;
                    scrollOffset = (scrollbar.y * totalHeight) / 610;
                }
            }
        }
        if (quit){
            break;
        }

        SDL_SetRenderDrawColor(ren, 0, 255, 255, 0);
        SDL_RenderClear(ren);

        {
            std::lock_guard<std::mutex> lock(weatherDataMutex);
            for (int i = 0; i < numItems; ++i) {
                int yPosition = i * itemHeight - scrollOffset;
                if (yPosition >= 640 || yPosition + itemHeight < 0) continue;

                addIcon(ren, iconPathList[i], 820, yPosition + 10, 100, 100);
                addText(ren, font, temperatureList[i], 920, yPosition + 10);
                addText(ren, font, descriptionList[i], 920, yPosition + 40);
                addText(ren, font, timeList[i], 920, yPosition + 70);
            }
        }

        addIcon(ren, "C:/Users/Wojciech Fortuna/Cpp_Project/settings-icon.png", 20, 20, 50, 50);

        SDL_SetRenderDrawColor(ren, 150, 150, 150, 255);
        SDL_RenderFillRect(ren, &scrollbar);

        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        SDL_RenderFillRect(ren, &closeButton);
        renderText(ren, font, "Close", closeButton);

        SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
        SDL_RenderFillRect(ren, &sdlBadDays);
        renderText(ren, font_big, "Prepare for " + std::to_string(bad_days) +
        " days with bad weather", sdlBadDays);

        SDL_RenderPresent(ren);


        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // White color for rectangle
        SDL_RenderFillRect(ren, &inputRect);

        renderTextInput(ren, font, inputText, inputRect.x + 10, inputRect.y + 10);

        SDL_Rect saveButton = {200, 400, 200, 50};
        SDL_SetRenderDrawColor(ren, 255, 255, 0, 255); // Yellow color for button
        SDL_RenderFillRect(ren, &saveButton);

        renderTextInput(ren, font, "Save", saveButton.x + 70, saveButton.y + 10);
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


