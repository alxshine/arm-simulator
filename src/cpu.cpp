#include "cpu.hpp"
#include "barrel_shifter.hpp"

#include <iostream>

using namespace std;

ARMSimulator::Cpu::Cpu() {
  for (int i = 0; i < mem_size; ++i) {
    mem[i] = 0;
  }
  for (int i = 0; i < 16; ++i) {
    regs[i] = 0;
  }

  Z = C = N = V = 0;
}

void ARMSimulator::Cpu::dumpRegisters() {
  cout << "ARM Registers:" << endl;

  for (int i = 0; i < 16; ++i) {
    cout << "r" << i;
    if (i == 14)
      cout << "/lr";
    if (i == 15)
      cout << "/pc";

    cout << ":\t0x" << hex << regs[i] << dec << endl;
  }
}

int ARMSimulator::Cpu::getRegister(Register r) { return regs[r]; }

void ARMSimulator::Cpu::setRegister(Register r, int value) { regs[r] = value; }

void ARMSimulator::Cpu::mov(Register rd, Register r1,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int input = getRegister(r1);
  auto shiftResult =
      ARMSimulator::BarrelShifter::executeConfig(input, shiftConfig);
  if (setFlags)
    C = shiftResult.carry;

  setRegister(rd, shiftResult.value);
}

void ARMSimulator::Cpu::mov(Register rd, int immediate,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  auto shiftResult =
      ARMSimulator::BarrelShifter::executeConfig(immediate, shiftConfig);
  if (setFlags)
    C = shiftResult.carry;

  setRegister(rd, shiftResult.value);
}
