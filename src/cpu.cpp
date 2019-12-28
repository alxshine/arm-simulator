#include "cpu.hpp"
#include "barrel_shifter.hpp"
#include "exceptions.hpp"

#include <iostream>

using namespace std;

bool isNegative(int v) { return v & (1 << 31); }

bool hasOverflow(int result, int v1, int v2) {
  if (isNegative(v1) != isNegative(v2)) return false;

  return isNegative(result) != isNegative(v1);
}

int getNegative(int input) { return ~input + 1; }

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
  auto shiftResult = BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = shiftResult.value;
  int result = v1 + v2 + 1;
  setRegister(rd, result);

  if (setFlags) {
    C = isNegative(v1) && isNegative(v2);
    V = hasOverflow(result, v1, v2);
    N = isNegative(result);
    Z = result == 0;
  }
}

void ARMSimulator::Cpu::ADD(Register rd, Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int v1 = getRegister(r1);
  int v2 = getRightHandOperandValue(op2);
  auto shiftResult = BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = shiftResult.value;
  int result = v1 + v2;
  setRegister(rd, result);

  if (setFlags) {
    C = isNegative(v1) && isNegative(v2);
    V = hasOverflow(result, v1, v2);
    N = isNegative(result);
    Z = result == 0;
  }
}

void ARMSimulator::Cpu::AND(Register rd, Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int v1 = getRegister(r1);
  int v2 = getRightHandOperandValue(op2);
  auto shiftResult = BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = shiftResult.value;

  int result = v1 & v2;
  setRegister(rd, result);

  if (setFlags) {
    if (shiftResult.affectCarry) C = shiftResult.carry;
    N = isNegative(result);
    Z = result == 0;
  }
}

void ARMSimulator::Cpu::B(int address) { setRegister(Register::pc, address); }

void ARMSimulator::Cpu::BIC(Register rd, Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int v1 = getRegister(r1);
  int v2 = getRightHandOperandValue(op2);
  auto shiftResult = BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = shiftResult.value;
  int result = v1 & ~v2;
  setRegister(rd, result);

  if (setFlags) {
    if (shiftResult.affectCarry) C = shiftResult.carry;
    N = isNegative(result);
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

void ARMSimulator::Cpu::CMN(Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig) {
  int v1 = getRegister(r1);
  int v2 = getRightHandOperandValue(op2);
  auto shiftResult = BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = shiftResult.value;
  int result = v1 + v2;

  N = isNegative(result);
  Z = result == 0;
  C = isNegative(v1) & isNegative(v2);
  V = hasOverflow(result, v1, v2);
}

void ARMSimulator::Cpu::CMP(Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig) {
  int v1 = getRegister(r1);
  int v2 = getRightHandOperandValue(op2);
  auto shiftResult = BarrelShifter::executeConfig(v2, shiftConfig);

  // this makes the flag checks work
  v2 = getNegative(shiftResult.value);
  int result = v1 + v2;

  N = isNegative(result);
  Z = result == 0;
  C = isNegative(v1) & isNegative(v2);
  V = hasOverflow(result, v1, v2);
}

void ARMSimulator::Cpu::EOR(Register rd, Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int v1 = getRegister(r1);
  int v2 = getRightHandOperandValue(op2);
  auto shiftResult = BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = shiftResult.value;
  int result = v1 ^ v2;
  setRegister(rd, result);

  if (setFlags) {
    N = isNegative(result);
    Z = result == 0;
    if (shiftResult.affectCarry) C = shiftResult.carry;
  }
}

void ARMSimulator::Cpu::LDM(Register, bool, bool, OffsetDirection,
                            IndexingMethod, std::vector<Register>) {
  throw NotImplementedException("LDM");
}

void ARMSimulator::Cpu::LDR(Register, Register, RightHandOperand, bool,
                            TransferQuantity, OffsetDirection, IndexingMethod,
                            BarrelShifterConfig) {
  throw NotImplementedException("LDR");
}

void ARMSimulator::Cpu::MOV(Register rd, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int input = getRightHandOperandValue(op2);

  auto shiftResult = BarrelShifter::executeConfig(input, shiftConfig);
  if (setFlags && shiftResult.affectCarry) C = shiftResult.carry;

  setRegister(rd, shiftResult.value);
}

void ARMSimulator::Cpu::MRS(Register) { throw NotImplementedException("MRS"); }

void ARMSimulator::Cpu::MSR(Register) { throw NotImplementedException("MSR"); }

void ARMSimulator::Cpu::MVN(Register rd, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int input = getRightHandOperandValue(op2);
  auto shiftResult = BarrelShifter::executeConfig(input, shiftConfig);
  int result = ~shiftResult.value;
  setRegister(rd, result);

  if (setFlags && shiftResult.affectCarry) C = shiftResult.carry;
}

void ARMSimulator::Cpu::ORR(Register rd, Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int v1 = getRegister(r1);
  int v2 = getRightHandOperandValue(op2);
  auto shiftResult = BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = shiftResult.value;
  int result = v1 | v2;
  setRegister(rd, result);

  if (setFlags) {
    N = isNegative(result);
    Z = result == 0;
    if (shiftResult.affectCarry) C = shiftResult.carry;
  }
}

void ARMSimulator::Cpu::RSB(Register rd, Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int v1 = getRegister(r1);
  v1 = getNegative(v1);

  int v2 = getRightHandOperandValue(op2);
  auto shiftResult = BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = shiftResult.value;

  int result = v2 + v1;
  setRegister(rd, result);

  if (setFlags) {
    N = isNegative(result);
    Z = result == 0;
    C = isNegative(v1) && isNegative(v2);
    V = hasOverflow(result, v1, v2);
  }
}

void ARMSimulator::Cpu::RSC(Register rd, Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int v1 = getRegister(r1);
  v1 = getNegative(v1) + 1;

  int v2 = getRightHandOperandValue(op2);
  auto shiftResult = BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = shiftResult.value;

  int result = v2 + v1;
  setRegister(rd, result);

  if (setFlags) {
    N = isNegative(result);
    Z = result == 0;
    C = isNegative(v1) && isNegative(v2);
    V = hasOverflow(result, v1, v2);
  }
}

void ARMSimulator::Cpu::SBC(Register rd, Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int v1 = getRegister(r1);
  int v2 = getRightHandOperandValue(op2);
  auto shiftResult = BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = getNegative(shiftResult.value) + 1;

  int result = v2 + v1;
  setRegister(rd, result);

  if (setFlags) {
    N = isNegative(result);
    Z = result == 0;
    C = isNegative(v1) && isNegative(v2);
    V = hasOverflow(result, v1, v2);
  }
}

void ARMSimulator::Cpu::STM(Register, bool, bool, OffsetDirection,
                            IndexingMethod, std::vector<Register>) {
  throw NotImplementedException("STM");
}

void ARMSimulator::Cpu::STR(Register, Register, RightHandOperand, bool,
                            TransferQuantity, OffsetDirection, IndexingMethod,
                            BarrelShifterConfig) {
  throw NotImplementedException("STR");
}

void ARMSimulator::Cpu::SUB(Register rd, Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int v1 = getRegister(r1);
  int v2 = getRightHandOperandValue(op2);
  auto shiftResult = BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = getNegative(shiftResult.value);

  int result = v2 + v1;
  setRegister(rd, result);

  if (setFlags) {
    N = isNegative(result);
    Z = result == 0;
    C = isNegative(v1) && isNegative(v2);
    V = hasOverflow(result, v1, v2);
  }
}

void ARMSimulator::Cpu::SWI() { throw NotImplementedException("SWI"); }

void ARMSimulator::Cpu::SWP(Register, Register, Register) {
  throw NotImplementedException("SWP");
}

void ARMSimulator::Cpu::TEQ(Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig) {
  int v1 = getRegister(r1);
  int v2 = getRightHandOperandValue(op2);
  auto shiftResult = BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = shiftResult.value;
  int result = v1 ^ v2;

  N = isNegative(result);
  Z = result == 0;
  if (shiftResult.affectCarry) C = shiftResult.carry;
}

void ARMSimulator::Cpu::TST(Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig) {
  int v1 = getRegister(r1);
  int v2 = getRightHandOperandValue(op2);
  auto shiftResult = BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = shiftResult.value;
  int result = v1 & v2;

  N = isNegative(result);
  Z = result == 0;
  if (shiftResult.affectCarry) C = shiftResult.carry;
}

int ARMSimulator::Cpu::getRightHandOperandValue(RightHandOperand operand) {
  if (operand.type == OperandType::Immediate)
    return operand.value.immediate;
  else
    return getRegister(operand.value.reg);
}