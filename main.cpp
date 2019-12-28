#include "barrel_shifter.hpp"
#include "cpu.hpp"
#include "type_definitions.hpp"

#include <iostream>

using namespace ARMSimulator;
using namespace std;

int main(void) {
  Cpu cpu;

  cpu.setRegister(Register::lr, 0x11);
  cpu.MOV(Register::r0, 0x80000000);
  cpu.AND(Register::r1, Register::r0, Register::r0, {ShiftType::LogicalLeft, 0},
          true);

  cpu.dumpRegisters();

  cpu.BX(0);
  return 0;
}
