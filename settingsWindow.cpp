#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "sdl_functions.h"

using json = nlohmann::json;

std::string apiKey = "d3a1b957fdd0c0e08c71578f47875320";

bool isDigit12345(std::string &str) {
    return str == "1" || str == "2" || str == "3" || str == "4" || str == "5";
}


static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool locationExists(std::string& location) {
    std::string url = "http://api.openweathermap.org/data/2.5/forecast?q=" + location +
            "&appid=" + apiKey + "&units=metric";
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return false;
        }
    }
    curl_global_cleanup();

    try {
        json response = json::parse(readBuffer);
        if (response.contains("cod") && response["cod"] == "200") {
            return true;
        } else {
            return false;
        }
    } catch (const json::parse_error& e) {
        std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
        return false;
    }
}


void createSettingsWindow(SDL_Event &e, int &forecastDays, std::string &location, bool &immediateChange) {
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

    SDL_Rect forecastInputRect = {50, 100, 300, 50};
    SDL_Rect locationInputRect = {50, 300, 300, 50};

    SDL_Rect forecastSaveRect = {350, 100, 150, 50};
    SDL_Rect locationSaveRect = {350, 300, 150, 50};

    SDL_Rect forecastErrorRect = {50, 150, 300, 50};
    SDL_Rect locationErrorRect = {50, 350, 300, 50};

    SDL_Rect forecastTextRect = {50, 50, 300, 50};
    SDL_Rect locationTextRect = {50, 250, 300, 50};


    bool settingsQuit = false;
    int CURSOR_BLINK_INTERVAL = 500;

    bool isForecastInputActive = false;
    bool cursorForecastActive = false;
    bool cursorForecastVisible = false;
    Uint32 lastForecastBlinkTime = 0;
    std::string forecastString = std::to_string(forecastDays);
    size_t cursorForecastPosition = forecastString.size();
    std::string forecastErrorMessage;

    bool isLocationInputActive = false;
    bool cursorLocationActive = false;
    bool cursorLocationVisible = false;
    Uint32 lastLocationBlinkTime = 0;
    std::string locationString = location;
    size_t cursorLocationPosition = locationString.size();
    std::string locationErrorMessage;

    SDL_Rect closeButton = {0, 430, 100, 50};


    SDL_Event settingsEvent;
    while (!settingsQuit) {
        while (SDL_PollEvent(&settingsEvent)) {
            if (settingsEvent.type == SDL_QUIT) {
                settingsQuit = true;
                break;
            }
            else if (settingsEvent.type == SDL_MOUSEBUTTONDOWN) {
                if (settingsEvent.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = settingsEvent.button.x;
                    int mouseY = settingsEvent.button.y;
                    if (insideRect(closeButton, mouseX, mouseY)) {
                        settingsQuit = true;
                        break;
                    }
                    if (insideRect(forecastInputRect, mouseX, mouseY)) {
                        SDL_StartTextInput();
                        isForecastInputActive = true;
                        cursorForecastActive = true;
                        isLocationInputActive = false;
                        cursorLocationActive = false;
                    }
                    else if (insideRect(locationInputRect, mouseX, mouseY)) {
                        SDL_StartTextInput();
                        isLocationInputActive = true;
                        cursorLocationActive = true;
                        isForecastInputActive = false;
                        cursorForecastActive = false;
                    }
                    else if (insideRect(forecastSaveRect, mouseX, mouseY)) {
                        isForecastInputActive = false;
                        cursorForecastActive = false;
                        isForecastInputActive = false;
                        cursorForecastActive = false;
                        if (isDigit12345(forecastString)){
                            forecastDays = std::stoi(forecastString);
                            forecastErrorMessage.clear();
                            immediateChange = true;
                        }
                        else{
                            forecastErrorMessage = "Only numbers from 1 to 5 are legal";
                            forecastString = std::to_string(forecastDays);
                        }
                    }
                    else if (insideRect(locationSaveRect, mouseX, mouseY)) {
                        isForecastInputActive = false;
                        cursorForecastActive = false;
                        isForecastInputActive = false;
                        cursorForecastActive = false;
                        if (locationExists(locationString)){
                            location = locationString;
                            locationErrorMessage.clear();
                            immediateChange = true;
                        }
                        else{
                            locationErrorMessage = "Unsupported location";
                            locationString = location;
                        }
                    }
                    else {
                        SDL_StopTextInput();
                        isForecastInputActive = false;
                        cursorForecastActive = false;
                        isLocationInputActive = false;
                        cursorLocationActive = false;
                    }
                }
            }
            else if (settingsEvent.type == SDL_TEXTINPUT) {
                if (isForecastInputActive) {
                    handleTextInput(settingsEvent, forecastString, cursorForecastPosition);
                }
                if (isLocationInputActive) {
                    handleTextInput(settingsEvent, locationString, cursorLocationPosition);
                }
            }
            else if (settingsEvent.type == SDL_KEYDOWN) {
                if (isForecastInputActive) {
                    if (settingsEvent.key.keysym.sym == SDLK_BACKSPACE && cursorForecastPosition > 0) {
                        forecastString.erase(cursorForecastPosition - 1, 1);
                        cursorForecastPosition--;
                    } else if (settingsEvent.key.keysym.sym == SDLK_LEFT && cursorForecastPosition > 0) {
                        cursorForecastPosition--;
                    } else if (settingsEvent.key.keysym.sym == SDLK_RIGHT && cursorForecastPosition < forecastString.length()) {
                        cursorForecastPosition++;
                    }
                }
                else if (isLocationInputActive) {
                    if (settingsEvent.key.keysym.sym == SDLK_BACKSPACE && cursorLocationPosition > 0) {
                        locationString.erase(cursorLocationPosition - 1, 1);
                        cursorLocationPosition--;
                    } else if (settingsEvent.key.keysym.sym == SDLK_LEFT && cursorLocationPosition > 0) {
                        cursorLocationPosition--;
                    } else if (settingsEvent.key.keysym.sym == SDLK_RIGHT && cursorLocationPosition < locationString.length()) {
                        cursorLocationPosition++;
                    }
                }
            }
        }
        Uint32 currentForecastTime = SDL_GetTicks();
        if (currentForecastTime - lastForecastBlinkTime >= CURSOR_BLINK_INTERVAL) {
            cursorForecastVisible = !cursorForecastVisible;
            lastForecastBlinkTime = currentForecastTime;
        }
        if (!cursorForecastActive) {
            cursorForecastVisible = false;
        }

        Uint32 currentLocationTime = SDL_GetTicks();
        if (currentLocationTime - lastLocationBlinkTime >= CURSOR_BLINK_INTERVAL) {
            cursorLocationVisible = !cursorLocationVisible;
            lastLocationBlinkTime = currentLocationTime;
        }
        if (!cursorLocationActive) {
            cursorLocationVisible = false;
        }

        SDL_SetRenderDrawColor(settingsRen, 0, 255, 255, 0);
        SDL_RenderClear(settingsRen);

        SDL_SetRenderDrawColor(settingsRen, 255, 255, 255, 255);
        SDL_RenderFillRect(settingsRen, &forecastInputRect);
        renderTextInput(settingsRen, font, forecastString, forecastInputRect.x + 10,
                        forecastInputRect.y + 10,cursorForecastVisible, cursorForecastPosition);

        SDL_SetRenderDrawColor(settingsRen, 255, 255, 255, 255);
        SDL_RenderFillRect(settingsRen, &locationInputRect);
        renderTextInput(settingsRen, font, locationString, locationInputRect.x + 10,
                        locationInputRect.y + 10,cursorLocationVisible, cursorLocationPosition);

        SDL_SetRenderDrawColor(settingsRen, 0, 255, 0, 255);
        SDL_RenderFillRect(settingsRen, &forecastSaveRect);
        renderText(settingsRen, font, "Save", forecastSaveRect);

        SDL_SetRenderDrawColor(settingsRen, 0, 255, 0, 255);
        SDL_RenderFillRect(settingsRen, &locationSaveRect);
        renderText(settingsRen, font, "Save", locationSaveRect);

        SDL_SetRenderDrawColor(settingsRen, 255, 255, 0, 255);
        SDL_RenderFillRect(settingsRen, &forecastTextRect);
        renderText(settingsRen, font, "Liczba dni:", forecastTextRect);

        SDL_SetRenderDrawColor(settingsRen, 255, 255, 0, 255);
        SDL_RenderFillRect(settingsRen, &locationTextRect);
        renderText(settingsRen, font, "Lokalizacja", locationTextRect);

        if (!forecastErrorMessage.empty()){
            SDL_SetRenderDrawColor(settingsRen, 255, 0, 0, 255);
            SDL_RenderFillRect(settingsRen, &forecastErrorRect);
            renderText(settingsRen, font, forecastErrorMessage, forecastErrorRect);
        }
        if (!locationErrorMessage.empty()) {
            SDL_SetRenderDrawColor(settingsRen, 255, 0, 0, 255);
            SDL_RenderFillRect(settingsRen, &locationErrorRect);
            renderText(settingsRen, font, locationErrorMessage, locationErrorRect);
        }

        closeButtonSetter(settingsRen, closeButton, font);

        SDL_RenderPresent(settingsRen);
    }

    SDL_DestroyRenderer(settingsRen);
    SDL_DestroyWindow(settingsWin);
}
