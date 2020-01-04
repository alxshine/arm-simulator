#pragma once
#include "constants.hpp"

namespace ARMSimulator {
  namespace BarrelShifter{
    BarrelShifterReturn logicalShiftRight(int input, short shiftAmount);
    BarrelShifterReturn arithmeticShiftRight(int input, short shiftAmount);
    BarrelShifterReturn logicalShiftLeft(int input, short shiftAmount);
    BarrelShifterReturn rotateRight(int input, short shiftAmount);

    BarrelShifterReturn executeConfig(int input, BarrelShifterConfig config);
  }
} // namespace ARMSimulator
