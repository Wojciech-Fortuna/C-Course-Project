#include <iostream>
#include <vector>
#include <unordered_map>

#include "nr_days.h"


int categorizeWeatherByDay(const std::vector<std::string>& timeList,
                           const std::vector<std::string>& mainDescriptionList) {

    std::unordered_map<std::string, bool> dayWithRain;

    for (size_t i = 0; i < timeList.size(); ++i) {
        std::string time = timeList[i];
        std::string hour = time.substr(11, 2);

        if (hour == "00" || hour == "03") {
            continue;
        }

        std::string day = time.substr(0, 10);
        std::string weatherDescription = mainDescriptionList[i];

        if (weatherDescription.find("rain") != std::string::npos) {
            dayWithRain[day] = true;
        }
        if (weatherDescription.find("thunderstorm") != std::string::npos) {
            dayWithRain[day] = true;
        }

    }

    return dayWithRain.size();
}