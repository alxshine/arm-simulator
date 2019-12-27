#pragma once
#include "type_definitions.hpp"

namespace ARMSimulator {

class Cpu {
 public:
  Cpu();
  void dumpRegisters();

  int getRegister(Register r);
  void setRegister(Register r, int value);

  void ADC(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);
  void ADD(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);
  void AND(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);
  void B(int address);
  void BIC(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);
  void BL(int address);
  void BX(RightHandOperand address);
  void CMN(Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0});
  void CMP(Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0});
  void EOR(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0},
           bool setFlags = false);
  // TODO: LDM
  void LDR(Register rd, Register baseRegister, RightHandOperand offset,
           bool addressWriteBack, TransferQuantity transferQuantity,
           OffsetDirection offsetDirection, IndexingMethod indexingMethod,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0});

  void MOV(Register rd, RightHandOperand op2,
           BarrelShifterConfig BarrelShifterConfig = {ShiftType::LogicalLeft,
                                                      0},
           bool setFlags = false);
  void MRS(Register rd);
  void MSR(Register r1);
  // TODO: MUL, MLA
  void MVN(Register rd, RightHandOperand op2,
           BarrelShifterConfig BarrelShifterConfig = {ShiftType::LogicalLeft,
                                                      0},
           bool setFlags = false);
  void ORR(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig BarrelShifterConfig = {ShiftType::LogicalLeft,
                                                      0},
           bool setFlags = false);
  void RSB(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig BarrelShifterConfig = {ShiftType::LogicalLeft,
                                                      0},
           bool setFlags = false);
  void RSC(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig BarrelShifterConfig = {ShiftType::LogicalLeft,
                                                      0},
           bool setFlags = false);
  void SBC(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig BarrelShifterConfig = {ShiftType::LogicalLeft,
                                                      0},
           bool setFlags = false);
  // TODO: STM
  void STR(Register rd, Register baseRegister, RightHandOperand offset,
           bool addressWriteBack, TransferQuantity transferQuantity,
           OffsetDirection offsetDirection, IndexingMethod indexingMethod,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0});
  void SUB(Register rd, Register r1, RightHandOperand op2,
           BarrelShifterConfig BarrelShifterConfig = {ShiftType::LogicalLeft,
                                                      0},
           bool setFlags = false);
  void SWI();
  void SWP(Register destinationRegister, Register sourceRegister,
           Register baseRegister);
  void TEQ(Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0});
  void TST(Register r1, RightHandOperand op2,
           BarrelShifterConfig shiftConfig = {ShiftType::LogicalLeft, 0});

 private:
  static const long mem_size = 4096;

  int regs[16];
  unsigned char mem[mem_size];
  bool Z, C, N, V;

  int getRightHandOperandValue(RightHandOperand operand);
};
}  // namespace ARMSimulator