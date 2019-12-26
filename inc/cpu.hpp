#pragma once
#include "type_definitions.hpp"

namespace ARMSimulator {

class Cpu {
private:
  static const long mem_size = 4096;

  int regs[16];
  unsigned char mem[mem_size];
  bool Z, C, N, V;

public:
  Cpu();
  void dumpRegisters();

  int getRegister(Register r);
  void setRegister(Register r, int value);

  void mov(Register rd, Register r1,
           BarrelShifterConfig shiftoConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);
  void mov(Register rd, int immediate,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);
  void add(Register rd, Register r1, Register r2,
           BarrelShifterConfig shiftconfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);
  void add(Register rd, Register r1, int immediate,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);
};

} // namespace ARMSimulator
