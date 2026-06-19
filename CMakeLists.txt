cmake_minimum_required(VERSION 3.16)
project(Mario CXX)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_DIR "C:/msys64/ucrt64/lib/cmake/SDL2")

find_package(SDL2 REQUIRED)

add_executable(mario src/main.cpp)

target_include_directories(mario PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(mario PRIVATE ${SDL2_LIBRARIES})