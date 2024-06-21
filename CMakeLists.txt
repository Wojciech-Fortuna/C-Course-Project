cmake_minimum_required(VERSION 3.14)
project(HappyWeather)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the path to the vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "C:/Users/Wojciech Fortuna/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Find and configure CURL
find_package(CURL REQUIRED)

# Find and configure nlohmann_json
find_package(nlohmann_json CONFIG REQUIRED)

# Find and configure SDL2
find_package(SDL2 CONFIG REQUIRED)

# Find and configure SDL2_image
find_package(SDL2_image CONFIG REQUIRED)

find_package(SDL2_ttf CONFIG REQUIRED)

# Add your source files here
add_executable(HappyWeather
        main.cpp
        sdl_window.cpp
        sdl_window.h
        weatherData.cpp
        weatherData.h
        settingsWindow.cpp
        settingsWindow.h
        settingsWindow.h
        sdl_functions.cpp
        sdl_functions.h
        nr_days.cpp
        nr_days.h
)

# Link libraries to the HappyWeather target
target_link_libraries(HappyWeather PRIVATE
        ${CURL_LIBRARIES}
        nlohmann_json::nlohmann_json
        SDL2::SDL2
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>

)

# Print a message indicating the use of vcpkg toolchain file
message("Using vcpkg toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
