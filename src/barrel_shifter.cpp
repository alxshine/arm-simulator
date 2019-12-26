#include "barrel_shifter.hpp"
#include "type_definitions.hpp"

ARMSimulator::BarrelShifterReturn
ARMSimulator::BarrelShifter::logicalShiftLeft(int input, short shiftAmount) {
  if (shiftAmount == 0)
    return {input, 0};

  bool carry = input & 1 << (32 - shiftAmount);
  return {input << shiftAmount, carry};
}

ARMSimulator::BarrelShifterReturn
ARMSimulator::BarrelShifter::logicalShiftRight(int input, short shiftAmount) {
  // lsr #0 is used to encode lsr #32
  if (shiftAmount == 0)
    shiftAmount = 32;

  bool carry = input & (1 << (shiftAmount - 1));
  return {input >> shiftAmount, carry};
}

ARMSimulator::BarrelShifterReturn
ARMSimulator::BarrelShifter::arithmeticShiftRight(int input,
                                                  short shiftAmount) {
  bool carry = input & (1 << (shiftAmount - 1));
  bool negative = input & (1 << 31);

  int result = input >> shiftAmount;
  if (negative) {
    int orMask = (1 << shiftAmount) - 1;
    result |= orMask << (32 - shiftAmount);
  }
  return {result, carry};
}
