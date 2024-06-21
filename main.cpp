#include <iostream>
#include <vector>

#include "sdl_window.h"
#include "weatherData.h"



int main()
{
    std::vector<std::string> iconPathList;
    std::vector<std::string> temperatureList;
    std::vector<std::string> descriptionList;
    std::vector<std::string> mainDescriptionList;
    std::vector<std::string> timeList;

    getCurrentData(iconPathList, temperatureList, descriptionList, mainDescriptionList, timeList);

    int exit_code = createMainWindow(iconPathList,temperatureList,
                                     descriptionList, mainDescriptionList,timeList);

    return exit_code;
}
