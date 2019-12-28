#pragma once

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
  lr = 14,
  pc = 15
};

enum class ShiftType {
  LogicalLeft,
  LogicalRight,
  ArithmeticRight,
  RotateRight
};

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

  RightHandOperand(int immediate)
      : type(OperandType::Immediate), value{immediate} {}

  RightHandOperand(Register reg) : type(OperandType::Register), value{reg} {}
};

enum OffsetDirection { Down = 0, Up = 1 };

enum TransferQuantity { Word = 0, Byte = 1 };

enum IndexingMethod { PostIndexed = 0, PreIndexed = 1 };
}  // namespace ARMSimulator
