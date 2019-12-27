#include "barrel_shifter.hpp"
#include "cpu.hpp"
#include "type_definitions.hpp"

#include <iostream>

using namespace ARMSimulator;
using namespace std;

int main(void) {
  Cpu cpu;

  cpu.setRegister(Register::lr, 0x11);
  cpu.mov(Register::r0, 0x1);
  cpu.add(Register::r1, Register::r0, Register::r0, {ShiftType::LogicalLeft, 1});

  cpu.dumpRegisters();
  return 0;
}
