#include "constants.hpp"
#include "cpu.hpp"
#include "exceptions.hpp"

#include <bitset>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

ARMSimulator::DataProcessingOperation
ARMSimulator::Cpu::decodeDataProcessing(bitset<32> bits) {
  if (!bits[24] && !bits[23] && !bits[22] && !bits[21])
    return DataProcessingOperation::BitwiseAND;
  if (!bits[24] && !bits[23] && !bits[22] && bits[21])
    return DataProcessingOperation::BitwiseEOR;
  if (!bits[24] && !bits[23] && bits[22] && !bits[21])
    return DataProcessingOperation::Subtract;
  if (!bits[24] && !bits[23] && bits[22] && bits[21])
    return DataProcessingOperation::ReverseSubtract;
  if (!bits[24] && bits[23] && !bits[22] && !bits[21])
    return DataProcessingOperation::Add;
  if (!bits[24] && bits[23] && !bits[22] && bits[21])
    return DataProcessingOperation::AddWithCarry;
  if (!bits[24] && bits[23] && bits[22] && !bits[21])
    return DataProcessingOperation::SubtractWithCarry;
  if (!bits[24] && bits[23] && bits[22] && bits[21])
    return DataProcessingOperation::ReverseSubtractWithCarry;

  if (bits[24] && !bits[23] && !bits[20]) {
    // data processing and miscellaneous instructions
    if (!bits[7])
      throw NotImplementedException(
          "Miscellaneous instructions not implemented");
    return DataProcessingOperation::HalfwordMultiply;
  }

  if (bits[24] && !bits[23] && !bits[22] && !bits[21])
    return DataProcessingOperation::Test;
  if (bits[24] && !bits[23] && !bits[22] && bits[21])
    return DataProcessingOperation::TestEquivalence;
  if (bits[24] && !bits[23] && bits[22] && !bits[21])
    return DataProcessingOperation::Compare;
  if (bits[24] && !bits[23] && bits[22] && bits[21])
    return DataProcessingOperation::CompareNegative;
  if (bits[24] && bits[23] && !bits[22] && !bits[21])
    return DataProcessingOperation::BitwiseOR;
  if (bits[24] && bits[23] && !bits[22] && bits[21])
    return DataProcessingOperation::Move;
  if (bits[24] && bits[23] && bits[22] && !bits[21])
    return DataProcessingOperation::BitwiseBitClear;
  if (bits[24] && bits[23] && bits[22] && bits[21])
    return DataProcessingOperation::BitwiseNOT;

  std::stringstream errorStream;
  errorStream << "Unknown Data Processing operation bits " << bits[24]
              << bits[23] << bits[22] << bits[21];
  throw NotImplementedException(errorStream.str());
}

void ARMSimulator::Cpu::executeInstruction(unsigned int instructionWord) {

  unsigned int condition = instructionWord >> FLAG_SHIFT;

  bool execute = true;
  switch (condition) {
  case EQ:
    execute = Z;
    break;
  case NE:
    execute = !Z;
    break;
  case CS:
    execute = C;
    break;
  case CC:
    execute = !C;
    break;
  case MI:
    execute = N;
    break;
  case PL:
    execute = !N;
    break;
  case VS:
    execute = V;
    break;
  case VC:
    execute = !V;
    break;
  case HI:
    execute = C && !Z;
    break;
  case LS:
    execute = !C || Z;
    break;
  case GE:
    execute = N == V;
    break;
  case LT:
    execute = N != V;
    break;
  case GT:
    execute = !Z && (N == V);
    break;
  case LE:
    execute = Z || (N != V);
    break;
  case AL:
    // always execute
    break;
  default:
    stringstream errorText;
    errorText << "Unknown condition code 0x" << hex << condition;
    throw NotImplementedException(errorText.str());
  }

  if (!execute) {
    regs[15] += 4; // increment PC
    return;
  }

  bitset<32> instructionBits{instructionWord};

  if (!instructionBits[27] && !instructionBits[26]) {
    cout << "Data processing" << endl;
    auto decodedOperation = decodeDataProcessing(instructionBits);
    bool setFlags = instructionBits[20];
    Register rn = getRegisterFromInt((instructionWord >> 16) & 0xF);
    Register rd = getRegisterFromInt((instructionWord >> 12) & 0xF);

    RightHandOperand op2;
    BarrelShifterConfig shiftConfig;

    if (!instructionBits[25]) {
      int rm = instructionWord & 0xF;
      op2 = RightHandOperand{OperandType::Register, rm};

      int shiftTypeBits = (instructionWord >> 5) & 0x3;
      shiftConfig.type = (ShiftType)shiftTypeBits;
      int shiftAmount = (instructionWord >> 7) & 0x1F;
      shiftConfig.shiftAmount = shiftAmount;
    } else {
      int value = instructionWord & 0x7F;
      op2 = {value};
      shiftConfig.type = RotateRight;
      shiftConfig.shiftAmount = (instructionWord >> 8) & 0x7;
    }

    switch (decodedOperation) {
    case DataProcessingOperation::BitwiseAND:
      AND(rd, rn, op2, shiftConfig, setFlags);
      break;
    case DataProcessingOperation::BitwiseEOR:
      EOR(rd, rn, op2, shiftConfig, setFlags);
      break;
    case DataProcessingOperation::Subtract:
      SUB(rd, rn, op2, shiftConfig, setFlags);
      break;
    case DataProcessingOperation::ReverseSubtract:
      RSB(rd, rn, op2, shiftConfig, setFlags);
      break;
    case DataProcessingOperation::Add:
      ADD(rd, rn, op2, shiftConfig, setFlags);
      break;
    case DataProcessingOperation::AddWithCarry:
      ADC(rd, rn, op2, shiftConfig, setFlags);
      break;
    case DataProcessingOperation::SubtractWithCarry:
      throw NotImplementedException(
          "SubtractWithCarry currently not implemented");
    case DataProcessingOperation::ReverseSubtractWithCarry:
      throw NotImplementedException(
          "ReverseSubtractWithCarry currently not implemented");
    case DataProcessingOperation::MiscellaneousInstruction:
      throw NotImplementedException(
          "Miscellaneous instructions currently not implemented");
    case DataProcessingOperation::HalfwordMultiply:
    case DataProcessingOperation::Multiply:
      throw NotImplementedException("Multiplication currently not implemented");
    case DataProcessingOperation::Test:
      TST(rn, op2);
      break;
    case DataProcessingOperation::TestEquivalence:
      TEQ(rn, op2);
      break;
    case DataProcessingOperation::Compare:
      CMP(rn, op2);
      break;
    case DataProcessingOperation::CompareNegative:
      CMN(rn, op2);
      break;
    case DataProcessingOperation::BitwiseOR:
      ORR(rd, rn, op2, shiftConfig, setFlags);
      break;
    case DataProcessingOperation::Move:
      // A2 encoding (MOVW) is not implemented on purpose
      MOV(rd, op2, shiftConfig, setFlags);
      break;
    case DataProcessingOperation::BitwiseBitClear:
      BIC(rd, rn, op2, shiftConfig, setFlags);
      break;
    case DataProcessingOperation::BitwiseNOT:
      MVN(rd, op2, shiftConfig, setFlags);
      break;
    default:
      throw NotImplementedException("Unknown operation " +
                                    to_string((int)decodedOperation));
      break;
    }
  } else if (!instructionBits[27] && instructionBits[26]) {
    if (instructionBits[25] && instructionBits[24])
      throw NotImplementedException(
          "Media instructions are not planned for now");

    cout << "Load/Store" << endl;
  } else if (instructionBits[27] && !instructionBits[26]) {
    cout << "Branching and block data transfer" << endl;
  } else if (instructionBits[27] && instructionBits[26]) {
    cout << "Coprocessor instructions and supervisor call" << endl;
  }
  // check
}