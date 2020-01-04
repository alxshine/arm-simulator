#pragma once

namespace ARMSimulator {
const int FLAG_SHIFT = 28;
const unsigned int OP_MASK = 0x0e000000;
const int INSTRUCTION_TYPE_SHIFT = 24;
const int OP_SHIFT = 24;

enum Conditions {
  EQ = 0,
  NE,
  CS,
  CC,
  MI,
  PL,
  VS,
  VC,
  HI,
  LS,
  GE,
  LT,
  GT,
  LE,
  AL
};

enum class DataProcessingOperation {
  BitwiseAND,
  BitwiseEOR,
  Subtract,
  ReverseSubtract,
  Add,
  AddWithCarry,
  SubtractWithCarry,
  ReverseSubtractWithCarry,
  MiscellaneousInstruction,
  HalfwordMultiply,
  Multiply,
  Test,
  TestEquivalence,
  Compare,
  CompareNegative,
  BitwiseOR,
  Move,
  BitwiseBitClear,
  BitwiseNOT,
  FormPCRelativeAddress
};
} // namespace ARMSimulator
