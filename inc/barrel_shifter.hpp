#pragma once
#include "type_definitions.hpp"

namespace ARMSimulator {
  namespace BarrelShifter{
    BarrelShifterReturn logicalShiftRight(int value, short shiftAmount);
    BarrelShifterReturn arithmeticShiftRight(int value, short shiftAmount);
    BarrelShifterReturn logicalShiftLeft(int value, short shiftAmount);
    BarrelShifterReturn rotateRight(int value, short shiftAmount);
  }
} // namespace ARMSimulator
