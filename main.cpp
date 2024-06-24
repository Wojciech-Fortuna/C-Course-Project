#include <iostream>
#include <vector>

#include "sdl_window.h"


int main()
{
    std::vector<std::string> iconPathList;
    std::vector<std::string> temperatureList;
    std::vector<std::string> descriptionList;
    std::vector<std::string> mainDescriptionList;
    std::vector<std::string> timeList;

    int exit_code = createMainWindow(iconPathList,temperatureList,
                                     descriptionList, mainDescriptionList,timeList);

    return exit_code;
}
