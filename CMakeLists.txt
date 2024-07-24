cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(game_of_life_project)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

add_executable( game_of_life 
    src/main.cpp
    src/form.ui
    src/gameOfLife.cpp
)
target_link_libraries(game_of_life PRIVATE Qt5::Core Qt5::Widgets Qt5::Gui)

