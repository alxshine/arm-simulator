cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(default_build_type "Release")
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# set the project name
project(ARMSimulator)


include_directories("inc")
add_subdirectory("src")

# add the executable
add_executable(arm_simulator main.cpp)

target_link_libraries(arm_simulator simulatorlib)
