#pragma once

namespace ARMSimulator {
enum Register {
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
  pc = 15,
};

enum class ShiftType {
  LogicalLeft,
  LogicalRight,
  ArithmeticRight,
  RotateRight
};

enum class ShiftOperandType { Immediate, Register };

struct BarrelShifterConfig {
  ShiftOperandType operandType;
  ShiftType type;
};
} // namespace ARMSimulator
