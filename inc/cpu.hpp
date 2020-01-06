#pragma once
#include "constants.hpp"

#include <bitset>
#include <vector>

namespace ARMSimulator {

class Cpu {
public:
  Cpu(unsigned int memorySize, bool isBigEndian = true);
  void dumpRegisters();

  int getRegister(Register r);
  void setRegister(Register r, int value);

  int getMemoryWord(unsigned int address);
  void setMemoryWord(unsigned int address, int word);
  unsigned char getMemoryByte(unsigned int address);
  void setMemoryByte(unsigned int address, unsigned char byte);
  void setMemory(unsigned int startAddress, const unsigned char *bytes,
                 int len);

  void ADC(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);

  void ADD(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);

  void AND(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);

  void B(int offset);

  void BIC(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);

  void BL(int offset);

  void BX(RightHandOperand address);

  void CMN(Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0});

  void CMP(Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0});

  void EOR(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);

  void LDM(Register baseRegister, bool addressWriteBack,
           OffsetDirection offsetDirection, IndexingMethod indexingMethod,
           std::vector<Register> registerList);

  void LDR(Register destinationRegister, Register baseRegister,
           RightHandOperand offset, bool addressWriteBack,
           TransferQuantity transferQuantity, OffsetDirection offsetDirection,
           IndexingMethod indexingMethod,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0});

  void MOV(Register rd, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);

  void MRS(Register rd);

  void MSR(Register r1);

  // TODO: MUL, MLA

  void MVN(Register rd, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);

  void ORR(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);

  void RSB(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);

  void RSC(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);

  void SBC(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);

  void STM(Register baseRegister, bool addressWriteBack,
           OffsetDirection offsetDirection, IndexingMethod indexingMethod,
           std::vector<Register> registerList);

  void STR(Register sourceRegister, Register baseRegister,
           RightHandOperand offset, bool addressWriteBack,
           TransferQuantity transferQuantity, OffsetDirection offsetDirection,
           IndexingMethod indexingMethod,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0});

  void SUB(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);

  void SWI();

  void SWP(Register destinationRegister, Register sourceRegister,
           Register baseRegister);

  void TEQ(Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0});

  void TST(Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0});

  bool executeInstruction(unsigned int instructionWord);
  void nextInstruction();

private:
  unsigned int memSize;
  int regs[16];
  std::vector<unsigned char> mem;
  bool N, Z, C, V, isBigEndian;

  int getRightHandOperandValue(RightHandOperand operand);
  static DataProcessingOperation decodeDataProcessing(std::bitset<32> bits);
  bool executeDataProcessingInstruction(unsigned int instructionWord);
  bool executeLoadStoreInstruction(unsigned int instructionWord);
};
} // namespace ARMSimulator
