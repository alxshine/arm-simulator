#include "constants.hpp"
#include "cpu.hpp"

#include <iostream>

void ARMSimulator::Cpu::nextInstruction(unsigned int instructionWord) {

  unsigned char condition = (instructionWord & FLAG_MASK) >> 27;

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
    std::cout << "Unknown condition code " << std::hex << (int)condition
              << std::endl;
    break;
  }

  if (execute) {
  }
  regs[15] += 4; // load next instruction
}
