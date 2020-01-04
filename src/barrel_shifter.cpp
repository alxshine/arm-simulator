#include "barrel_shifter.hpp"
#include "constants.hpp"

ARMSimulator::BarrelShifterReturn ARMSimulator::BarrelShifter::logicalShiftLeft(
    int input, short shiftAmount) {
  if (shiftAmount == 0) return {input, 0, true};

  bool carry = input & 1 << (32 - shiftAmount);
  return {input << shiftAmount, carry};
}

ARMSimulator::BarrelShifterReturn
ARMSimulator::BarrelShifter::logicalShiftRight(int input, short shiftAmount) {
  // lsr #0 is used to encode lsr #32
  if (shiftAmount == 0) shiftAmount = 32;

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

ARMSimulator::BarrelShifterReturn ARMSimulator::BarrelShifter::rotateRight(
    int input, short shiftAmount) {
  bool carry = input & 1 << (shiftAmount - 1);
  int result = input >> shiftAmount | input << (32 - shiftAmount);
  return {result, carry};
}

ARMSimulator::BarrelShifterReturn ARMSimulator::BarrelShifter::executeConfig(
    int input, BarrelShifterConfig config) {
  switch (config.type) {
    case ARMSimulator::ShiftType::LogicalLeft:
      return logicalShiftLeft(input, config.shiftAmount);
    case ARMSimulator::ShiftType::LogicalRight:
      return logicalShiftRight(input, config.shiftAmount);
    case ARMSimulator::ShiftType::ArithmeticRight:
      return arithmeticShiftRight(input, config.shiftAmount);
    case ARMSimulator::ShiftType::RotateRight:
      return rotateRight(input, config.shiftAmount);
    default:
      return {0, 0};
  }
}
