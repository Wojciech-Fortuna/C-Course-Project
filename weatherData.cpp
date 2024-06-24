#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <algorithm>


using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void getWeatherData(std::string& apiKey, std::string& location,
                    std::vector<std::string>& iconCodeList, std::vector<std::string>& temperatureList,
                    std::vector<std::string>& descriptionList, std::vector<std::string>& mainDescriptionList,
                    std::vector<std::string>& timeList)
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    if(curl) {
        std::string url = "http://api.openweathermap.org/data/2.5/forecast?q=" +
                          location + "&appid=" + apiKey + "&units=metric";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            try {
                auto jsonData = json::parse(readBuffer);
                for (auto& day : jsonData["list"]) {
                    std::string iconCode = day["weather"][0]["icon"];
                    iconCodeList.push_back(iconCode);

                    double tempDouble = day["main"]["temp"];
                    std::string temperature = std::to_string(static_cast<int>(std::round(tempDouble)));
                    temperature += "\xB0";
                    temperature += "C";
                    temperatureList.push_back(temperature);

                    std::string description = day["weather"][0]["description"];
                    descriptionList.push_back(description);

                    std::string main_description = day["weather"][0]["main"];
                    mainDescriptionList.push_back(main_description);

                    std::string time = day["dt_txt"];
                    time.erase(time.size() - 3);
                    timeList.push_back(time);
                }
            } catch (json::parse_error& e) {
                std::cerr << "JSON parse error: " << e.what() << std::endl;
            }
        }
    }
    else {
        std::cerr << "curl wasn't initialized properly" << std::endl;
    }
}

size_t WriteDataToFile(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    return fwrite(ptr, size, nmemb, stream);
}

void downloadIcon(const std::string& iconCode, const std::string& iconPath) {
    CURL* curl;
    FILE* fp;
    CURLcode res;
    std::string url = "http://openweathermap.org/img/wn/" + iconCode + "@2x.png";

    curl = curl_easy_init();
    if (curl) {
        fp = fopen(iconPath.c_str(), "wb");
        if (!fp) {
            std::cerr << "Failed to open file for writing: " << iconPath << std::endl;
            curl_easy_cleanup(curl);
            return;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteDataToFile);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        fclose(fp);
        curl_easy_cleanup(curl);
    }
}

void sliceList(std::vector<std::string>& newList, std::vector<std::string>& oldList, int forecastDays){
    auto begin = std::make_move_iterator(oldList.begin());
    size_t minSize = std::min(static_cast<size_t>(forecastDays * 8), oldList.size());
    auto end = std::make_move_iterator(oldList.begin() + minSize);
    newList.insert(newList.begin(), begin, end);
}

void getCurrentData(std::vector<std::string>& iconPathList_gl, std::vector<std::string>& temperatureList_gl,
                    std::vector<std::string>& descriptionList_gl, std::vector<std::string>& mainDescriptionList_gl,
                    std::vector<std::string>& timeList_gl, int forecastDays, std::string& location)
{
    std::string apiKey = "d3a1b957fdd0c0e08c71578f47875320";

    std::vector<std::string> iconCodeList;
    std::vector<std::string> temperatureList;
    std::vector<std::string> descriptionList;
    std::vector<std::string> mainDescriptionList;
    std::vector<std::string> timeList;
    std::vector<std::string> iconPathList;

    getWeatherData(apiKey, location, iconCodeList, temperatureList, descriptionList, mainDescriptionList, timeList);

    int nr = 0;
    for (const auto& iconCode : iconCodeList) {
        std::string iconPath = "weather_icon_" + std::to_string(nr) + ".png";
        downloadIcon(iconCode, iconPath);
        iconPathList.push_back(iconPath);
        nr++;
    }

    iconPathList_gl.clear();
    sliceList(iconPathList_gl, iconPathList, forecastDays);
    temperatureList_gl.clear();
    sliceList(temperatureList_gl, temperatureList, forecastDays);
    descriptionList_gl.clear();
    sliceList(descriptionList_gl, descriptionList, forecastDays);
    mainDescriptionList_gl.clear();
    sliceList(mainDescriptionList_gl, mainDescriptionList, forecastDays);
    timeList_gl.clear();
    sliceList(timeList_gl, timeList, forecastDays);
}
