cmake_minimum_required(VERSION 3.10)
project(miniRenderer)

find_package(SDL2 CONFIG REQUIRED)

file(GLOB_RECURSE DC_SRC "miniRenderer/*.cpp" "miniRenderer/*.h")

add_executable(${PROJECT_NAME} ${DC_SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)