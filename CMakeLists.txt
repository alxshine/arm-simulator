cmake_minimum_required(VERSION 3.10)

project(arm_simulator)

set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(default_build_type "Debug")
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

include_directories("inc")
add_subdirectory("src")

# add the executable
add_executable(arm_simulator main.cpp)

target_link_libraries(arm_simulator simulatorlib)
