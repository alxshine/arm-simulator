#include "barrel_shifter.hpp"
#include "cpu.hpp"
#include "type_definitions.hpp"

#include <iostream>

using namespace ARMSimulator;
using namespace std;

int main(void) {
  Cpu cpu;

  cpu.setRegister(lr, 0x11);
  BarrelShifterConfig config = {ShiftType::RotateRight, 2};
  cpu.mov(r0, 0x11, config);
  cpu.mov(r1, r0, {ShiftType::LogicalLeft, 1});

  cpu.dumpRegisters();
  return 0;
}
