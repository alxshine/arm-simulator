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

  void mov(Register rd, Register r1, BarrelShifterConfig shifterConfig,
           bool setFlags);
  void mov(Register rd, int immediate, BarrelShifterConfig shifterConfig,
           bool setFlags);
  void add(Register rd, Register r1, Register r2,
           BarrelShifterConfig shifterconfig, bool setFlags);
  void add(Register rd, Register r1, int immediate,
           BarrelShifterConfig shifterConfig, bool setFlags);
};

} // namespace ARMSimulator
