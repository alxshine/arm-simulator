#include "barrel_shifter.hpp"
#include "cpu.hpp"
#include "type_definitions.hpp"

#include <iostream>

using namespace ARMSimulator;
using namespace std;

int main(void) {
  Cpu cpu;

  cpu.setRegister(lr, 0x11);
  cpu.mov(r0, 0x1);
  cpu.add(r1, r0, 2, {ShiftType::LogicalLeft, 1});

  cpu.dumpRegisters();
  return 0;
}
