#include "cpu.hpp"
#include "barrel_shifter.hpp"
#include "exceptions.hpp"

#include <iostream>

using namespace std;

bool isNegative(int v) { return v & (1 << 31); }

bool hasOverflow(int result, int v1, int v2) {
  if (isNegative(v1) != isNegative(v2))
    return false;

  return isNegative(result) != isNegative(v1);
}

int getNegative(int input) { return ~input + 1; }

ARMSimulator::Cpu::Cpu(unsigned int memorySize)
    : memSize{memorySize}, mem(memorySize, 0) {
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
  // N Z C V
  cout << "Flags:" << endl
       << "N: " << N << ", Z: " << Z << ", C: " << C << ", V: " << V << endl;
}

int ARMSimulator::Cpu::getRegister(Register r) {
  int intRegister = static_cast<int>(r);

  if (r == Register::pc) {
    return regs[15] +
           8; // when the PC is retrieved, current instruction + 8 is returned
    // TODO: if pc is used as an operand in a shift, it's ahead by 12
  }
  return regs[intRegister];
}

void ARMSimulator::Cpu::setRegister(Register r, int value) {
  int intRegister = static_cast<int>(r);
  regs[intRegister] = value;
}

int ARMSimulator::Cpu::getMemoryWord(unsigned int address) {
  if (address >= memSize - 4)
    throw InvalidMemoryAccessException(address);

  unsigned int b0 = mem[address] << 24;
  unsigned int b1 = mem[address + 1] << 16;
  unsigned int b2 = mem[address + 2] << 8;
  unsigned int b3 = mem[address + 3];
  unsigned int result = b0 | b1 | b2 | b3;
  return result;
}

void ARMSimulator::Cpu::setMemoryWord(unsigned int address, int value) {
  if (address >= memSize - 4)
    throw InvalidMemoryAccessException(address);

  mem[address] = value >> 24;
  mem[address + 1] = value >> 16 & 0xFF;
  mem[address + 2] = value >> 8 & 0xFF;
  mem[address + 3] = value & 0xFF;
}

unsigned char ARMSimulator::Cpu::getMemoryByte(unsigned int address) {
  if (address >= memSize)
    throw InvalidMemoryAccessException(address);

  return mem[address];
}

void ARMSimulator::Cpu::setMemoryByte(unsigned int address,
                                      unsigned char value) {
  if (address >= memSize)
    throw InvalidMemoryAccessException(address);

  mem[address] = value;
}

void ARMSimulator::Cpu::setMemory(unsigned int startAddress,
                                  const unsigned char *bytes, int len) {
  for (int i = 0; i < len; i++) {
    unsigned int currentAddress = startAddress + i;
    setMemoryByte(currentAddress, bytes[i]);
  }
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
    if (shiftResult.affectCarry)
      C = shiftResult.carry;
    N = isNegative(result);
    Z = result == 0;
  }
}

void ARMSimulator::Cpu::B(unsigned int address) {
  setRegister(Register::pc, address);
}

void ARMSimulator::Cpu::BIC(Register rd, Register r1, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int v1 = getRegister(r1);
  int v2 = getRightHandOperandValue(op2);
  auto shiftResult = BarrelShifter::executeConfig(v2, shiftConfig);
  v2 = shiftResult.value;
  int result = v1 & ~v2;
  setRegister(rd, result);

  if (setFlags) {
    if (shiftResult.affectCarry)
      C = shiftResult.carry;
    N = isNegative(result);
    Z = result == 0;
  }
}

void ARMSimulator::Cpu::BL(unsigned int address) {
  int currentProgramCounter = getRegister(Register::pc);
  // store address of next instruction so the return doesn't mess things up
  // after getRegister, pc is 8 ahead of current instruction
  int targetProgramCounter = currentProgramCounter - 4;
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
    if (shiftResult.affectCarry)
      C = shiftResult.carry;
  }
}

void ARMSimulator::Cpu::LDM(Register baseRegister, bool addressWriteBack,
                            OffsetDirection offsetDirection,
                            IndexingMethod indexingMethod,
                            std::vector<Register> registerList) {
  unsigned int targetAddress = getRegister(baseRegister);
  int offset;
  if (offsetDirection == Up)
    offset = 4;
  else
    offset = -4;

  for (Register r : registerList) {
    if (indexingMethod == PreIndexed)
      targetAddress += offset;
    int value = getMemoryWord(targetAddress);
    setRegister(r, value);
    if (indexingMethod == PostIndexed)
      targetAddress += offset;
  }

  if (addressWriteBack)
    setRegister(baseRegister, targetAddress);
}

void ARMSimulator::Cpu::LDR(Register destinationRegister, Register baseRegister,
                            RightHandOperand offsetOperand,
                            bool addressWriteBack,
                            TransferQuantity transferQuantity,
                            OffsetDirection offsetDirection,
                            IndexingMethod indexingMethod,
                            BarrelShifterConfig shiftConfig) {
  unsigned int baseAddress = getRegister(baseRegister);
  int offset = getRightHandOperandValue(offsetOperand);
  offset = BarrelShifter::executeConfig(offset, shiftConfig).value;
  if (baseRegister == Register::pc)
    offset +=
        4; // this is a hack for weird behaviour when addressing relative to PC
  
  unsigned int targetAddress = baseAddress;

  if (indexingMethod == PreIndexed) {
    if (offsetDirection == Up)
      targetAddress = baseAddress + offset;
    else
      targetAddress = baseAddress - offset;
  }

  int value;
  if (transferQuantity == Word)
    value = getMemoryWord(targetAddress);
  else
    value = getMemoryByte(targetAddress);
  setRegister(destinationRegister, value);

  if (indexingMethod == PostIndexed) {
    if (offsetDirection == Up)
      targetAddress = baseAddress + offset;
    else
      targetAddress = baseAddress - offset;
  }

  if (addressWriteBack)
    setRegister(baseRegister, targetAddress);
}

void ARMSimulator::Cpu::MOV(Register rd, RightHandOperand op2,
                            BarrelShifterConfig shiftConfig, bool setFlags) {
  int input = getRightHandOperandValue(op2);

  auto shiftResult = BarrelShifter::executeConfig(input, shiftConfig);
  if (setFlags && shiftResult.affectCarry)
    C = shiftResult.carry;

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

  if (setFlags && shiftResult.affectCarry)
    C = shiftResult.carry;
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
    if (shiftResult.affectCarry)
      C = shiftResult.carry;
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

void ARMSimulator::Cpu::STM(Register baseRegister, bool addressWriteBack,
                            OffsetDirection offsetDirection,
                            IndexingMethod indexingMethod,
                            std::vector<Register> registerList) {
  unsigned int targetAddress = getRegister(baseRegister);
  int offset;
  if (offsetDirection == Up)
    offset = 4;
  else
    offset = -4;

  for (Register r : registerList) {
    int value = getRegister(r);

    if (indexingMethod == PreIndexed)
      targetAddress += offset;
    setMemoryWord(targetAddress, value);
    if (indexingMethod == PostIndexed)
      targetAddress += offset;
  }

  if (addressWriteBack)
    setRegister(baseRegister, targetAddress);
}

void ARMSimulator::Cpu::STR(Register sourceRegister, Register baseRegister,
                            RightHandOperand offsetOperand,
                            bool addressWriteBack,
                            TransferQuantity transferQuantity,
                            OffsetDirection offsetDirection,
                            IndexingMethod indexingMethod,
                            BarrelShifterConfig shiftConfig) {
  unsigned int baseAddress = getRegister(baseRegister);
  int offset = getRightHandOperandValue(offsetOperand);
  offset = BarrelShifter::executeConfig(offset, shiftConfig).value;
  if (baseRegister == Register::pc)
    offset +=
        4; // this is a hack for weird behaviour when addressing relative to PC
  unsigned int targetAddress = baseAddress;

  if (indexingMethod == PreIndexed) {
    if (offsetDirection == Up)
      targetAddress = baseAddress + offset;
    else
      targetAddress = baseAddress - offset;
  }

  int value = getRegister(sourceRegister);
  if (transferQuantity == Word)
    setMemoryWord(targetAddress, value);
  else
    setMemoryByte(targetAddress, value);

  if (indexingMethod == PostIndexed) {
    if (offsetDirection == Up)
      targetAddress = baseAddress + offset;
    else
      targetAddress = baseAddress - offset;
  }

  if (addressWriteBack)
    setRegister(baseRegister, targetAddress);
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

void ARMSimulator::Cpu::SWI() {
  int syscall = getRegister(Register::r7);
  if (syscall == 4) {
    // print syscall
    int output = getRegister(Register::r0);
    unsigned int address = getRegister(Register::r1);
    int length = getRegister(Register::r2);

    for (int i = 0; i < length; i++) {
      char currentChar = getMemoryByte(address);
      if (currentChar == 0)
        break;

      if (output == 0)
        std::cout << currentChar;
      else
        std::cerr << currentChar;

      address++;
    }
  } else if (syscall == 1) {
    // exit syscall
    int returnCode = getRegister(Register::r0);
    throw returnCode;
  } else {
    throw NotImplementedException("Syscall " + std::to_string(syscall) +
                                  " not implemented");
  }
}

void ARMSimulator::Cpu::SWP(Register destinationRegister,
                            Register sourceRegister, Register baseRegister) {
  unsigned int address = getRegister(baseRegister);

  int memValue = getMemoryWord(address);
  setRegister(destinationRegister, memValue);

  int sourceVal = getRegister(sourceRegister);
  setMemoryWord(address, sourceVal);
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
  if (shiftResult.affectCarry)
    C = shiftResult.carry;
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
  if (shiftResult.affectCarry)
    C = shiftResult.carry;
}

int ARMSimulator::Cpu::getRightHandOperandValue(RightHandOperand operand) {
  if (operand.type == OperandType::Immediate) {
    return operand.value.immediate;
  } else {
    return getRegister(operand.value.reg);
  }
}
