add_library(simulatorlib
  cpu.cpp
  instruction_decoder.cpp
  barrel_shifter.cpp
)
target_include_directories(simulatorlib PUBLIC ../inc)

