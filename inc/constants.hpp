#pragma once

#include <bitset>
#include <vector>

namespace ARMSimulator {
enum class Register {
  r0 = 0,
  r1,
  r2,
  r3,
  r4,
  r5,
  r6,
  r7,
  r8,
  r9,
  r10,
  r11,
  r12,
  r13,
  r14,
  r15,
  // special registers
  sp = 13,
  lr = 14,
  pc = 15
};

Register getRegisterFromInt(int intReg);
std::vector<Register> parseRegisterList(std::bitset<16> registerBits);

enum ShiftType { LogicalLeft, LogicalRight, ArithmeticRight, RotateRight };

struct BarrelShifterConfig {
  ShiftType type;
  short shiftAmount;
};

struct BarrelShifterReturn {
  int value;
  bool carry;
  bool affectCarry;

  BarrelShifterReturn(int value, bool carry, bool affectCarry = true)
      : value{value}, carry{carry}, affectCarry{affectCarry} {}
};

union Op2Value {
  int immediate;
  Register reg;

  Op2Value(int immediate) : immediate(immediate) {}
  Op2Value(Register reg) : reg(reg) {}
};

enum class OperandType { Register, Immediate };

struct RightHandOperand {
  OperandType type;
  union Op2Value value;

  RightHandOperand() : type{OperandType::Immediate}, value{0} {}

  RightHandOperand(int immediate)
      : type(OperandType::Immediate), value{immediate} {}

  RightHandOperand(Register reg) : type(OperandType::Register), value{reg} {}

  RightHandOperand(OperandType type, int val) : type{type}, value{val} {}
};

enum OffsetDirection { Down = 0, Up = 1 };

enum TransferQuantity { Word = 0, Byte = 1 };

enum IndexingMethod { PostIndexed = 0, PreIndexed = 1 };

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

const char ELF_MAGIC_NUMBER[] = {0x7f, 0x45, 0x4c, 0x46};
const int ELF_HEADER_SIZE = 0x34;

const unsigned int BASE_ADDRESS = 0x10000;

using imm12 = short;
int signedExtend(imm12 value);
} // namespace ARMSimulator
