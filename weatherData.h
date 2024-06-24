#ifndef HAPPYWEATHER_WEATHERDATA_H
#define HAPPYWEATHER_WEATHERDATA_H


void getCurrentData(std::vector<std::string>& iconPathList_gl, std::vector<std::string>& temperatureList_gl,
                    std::vector<std::string>& descriptionList_gl, std::vector<std::string>& mainDescriptionList,
                    std::vector<std::string>& timeList_gl, int forecastDays, std::string& location);


#endif //HAPPYWEATHER_WEATHERDATA_H
