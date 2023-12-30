cmake_minimum_required(VERSION 3.23)
project(raycaster_c C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "C:/Users/boydj/libs/SDL2-2.28.5/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL_INCLUDE_DIR})

add_executable(raycaster_c main.c)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
