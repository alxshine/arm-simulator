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

void ARMSimulator::Cpu::add(Register rd, Register r1, Register r2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int v1 = getRegister(r1);
  int v2 = getRegister(r2);
  auto shiftResult =
      ARMSimulator::BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = shiftResult.value;
  int result = v1 + v2;
  setRegister(rd, result);

  if (setFlags) {
    int carryMask = 1 << 31;
    int overFlowMask = 1 << 30;

    C = !(result & carryMask) && (v1 & carryMask || v2 & carryMask);
    V = !(result & overFlowMask) && (v1 & overFlowMask || v2 & overFlowMask);
  }
}

void ARMSimulator::Cpu::add(Register rd, Register r1, int immediate, BarrelShifterConfig shiftConfig, bool setFlags){
  int v1 = getRegister(r1);
  auto shiftResult = ARMSimulator::BarrelShifter::executeConfig(immediate, shiftConfig);
  int v2 = shiftResult.value;
  int result = v1 + v2;
  setRegister(rd, result);

  if(setFlags){
    int carryMask = 1 << 31;
    int overFlowMask = 1 << 30;

    C = !(result & carryMask) && (v1 & carryMask || v2 & carryMask);
    V = !(result & overFlowMask) && (v1 & overFlowMask || v2 & overFlowMask);
  }
}
