#include "cpu.hpp"
#include "barrel_shifter.hpp"
#include "exceptions.hpp"

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
    if (i == 14) cout << "/lr";
    if (i == 15) cout << "/pc";

    cout << ":\t0x" << hex << regs[i] << dec << endl;
  }
  // N Z C V
  cout << "Flags:" << endl
       << "N: " << N << ", Z: " << Z << ", C: " << C << ", V: " << V << endl;
}

int ARMSimulator::Cpu::getRegister(Register r) {
  int intRegister = static_cast<int>(r);
  return regs[intRegister];
}

void ARMSimulator::Cpu::setRegister(Register r, int value) {
  int intRegister = static_cast<int>(r);
  regs[intRegister] = value;
}

void ARMSimulator::Cpu::ADC(Register rd, Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int v1 = getRegister(r1);
  int v2 = getRightHandOperandValue(op2);
  auto shiftResult =
      ARMSimulator::BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = shiftResult.value;
  int result = v1 + v2 + 1;
  setRegister(rd, result);

  if (setFlags) {
    int carryMask = 1 << 31;
    int overFlowMask = 1 << 30;

    C = !(result & carryMask) && (v1 & carryMask || v2 & carryMask);
    V = !(result & overFlowMask) && (v1 & overFlowMask || v2 & overFlowMask);
    N = result & carryMask;
    Z = result == 0;
  }
}

void ARMSimulator::Cpu::ADD(Register rd, Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int v1 = getRegister(r1);
  int v2 = getRightHandOperandValue(op2);
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
    N = result & carryMask;
    Z = result == 0;
  }
}

void ARMSimulator::Cpu::AND(Register rd, Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int v1 = getRegister(r1);
  int v2 = getRightHandOperandValue(op2);
  auto shiftResult =
      ARMSimulator::BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = shiftResult.value;

  int result = v1 & v2;
  setRegister(rd, result);

  if (setFlags) {
    if (shiftResult.affectCarry) C = shiftResult.carry;
    N = result & (1 << 31);
    Z = result == 0;
  }
}

void ARMSimulator::Cpu::B(int address) { setRegister(Register::pc, address); }

void ARMSimulator::Cpu::BIC(Register rd, Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int v1 = getRegister(r1);
  int v2 = getRightHandOperandValue(op2);
  auto shiftResult =
      ARMSimulator::BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = shiftResult.value;
  int result = v1 & ~v2;
  setRegister(rd, result);

  if (setFlags) {
    if (shiftResult.affectCarry) C = shiftResult.carry;
    N = result & (1 << 31);
    Z = result == 0;
  }
}

void ARMSimulator::Cpu::BL(int address) {
  int currentProgramCounter = getRegister(Register::pc);
  // store address of next instruction so the return doesn't mess things up
  int targetProgramCounter = currentProgramCounter + 4;
  setRegister(Register::lr, targetProgramCounter);
  setRegister(Register::pc, address);
}

void ARMSimulator::Cpu::BX(RightHandOperand) {
  throw NotImplementedException("BX");
}

void ARMSimulator::Cpu::MOV(Register rd, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int input;
  if (op2.type == OperandType::Immediate)
    input = op2.value.immediate;
  else
    input = getRegister(op2.value.reg);

  auto shiftResult =
      ARMSimulator::BarrelShifter::executeConfig(input, shiftConfig);
  if (setFlags) C = shiftResult.carry;

  setRegister(rd, shiftResult.value);
}

int ARMSimulator::Cpu::getRightHandOperandValue(RightHandOperand operand) {
  if (operand.type == OperandType::Immediate)
    return operand.value.immediate;
  else
    return getRegister(operand.value.reg);
}