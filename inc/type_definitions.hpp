#pragma once

namespace ARMSimulator
{
enum class Register
{
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

enum class ShiftType
{
  LogicalLeft,
  LogicalRight,
  ArithmeticRight,
  RotateRight
};

struct BarrelShifterConfig
{
  ShiftType type;
  short shiftAmount;
};

struct BarrelShifterReturn
{
  int value;
  bool carry;
};

union Op2Value {
  int immediate;
  Register reg;

  Op2Value(int immediate) : immediate(immediate) {}
  Op2Value(Register reg) : reg(reg) {}
};

enum class OperandType
{
  Register,
  Immediate
};

struct RightHandOperand
{
  OperandType type;
  union Op2Value value;

  RightHandOperand(int immediate)
      : type(OperandType::Immediate), value{immediate} {}

  RightHandOperand(Register reg) : type(OperandType::Register), value{reg} {}
};
} // namespace ARMSimulator
