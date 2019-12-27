#pragma once
#include "type_definitions.hpp"

namespace ARMSimulator {

class Cpu {

public:
  Cpu();
  void dumpRegisters();

  int getRegister(Register r);
  void setRegister(Register r, int value);

  void mov(Register rd, RightHandOperand op2,
           BarrelShifterConfig BarrelShifterConfig = {ShiftType::LogicalLeft,
                                                      0},
           bool setFlags = false);
  void add(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);

private:
  static const long mem_size = 4096;

  int regs[16];
  unsigned char mem[mem_size];
  bool Z, C, N, V;

  int getRightHandOperandValue(RightHandOperand operand);
};
} // namespace ARMSimulator