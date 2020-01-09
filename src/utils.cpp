#include "constants.hpp"
#include "exceptions.hpp"

#include <bitset>
#include <vector>

using namespace ARMSimulator;

Register ARMSimulator::getRegisterFromInt(int intReg) {
  switch (intReg) {
  case 0:
    return Register::r0;
  case 1:
    return Register::r1;
  case 2:
    return Register::r2;
  case 3:
    return Register::r3;
  case 4:
    return Register::r4;
  case 5:
    return Register::r5;
  case 6:
    return Register::r6;
  case 7:
    return Register::r7;
  case 8:
    return Register::r8;
  case 9:
    return Register::r9;
  case 10:
    return Register::r10;
  case 11:
    return Register::r11;
  case 12:
    return Register::r12;
  case 13:
    return Register::r13;
  case 14:
    return Register::r14;
  case 15:
    return Register::r15;
  default:
    throw InvalidRegisterException(intReg);
  }
}

std::vector<Register>
ARMSimulator::parseRegisterList(std::bitset<16> registerBits) {
  std::vector<Register> ret;
  for (int i = 0; i < 16; ++i) {
    if (registerBits[i])
      ret.push_back(getRegisterFromInt(i));
  }
  return ret;
}

int ARMSimulator::signedExtend(imm12 value) {
  int orMask = 0;
  if (value & 0x800)
    orMask = 0xFFFFF000;

  return (int)value | orMask;
}
