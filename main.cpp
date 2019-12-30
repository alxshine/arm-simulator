#include "barrel_shifter.hpp"
#include "cpu.hpp"
#include "type_definitions.hpp"

#include <iostream>

using namespace ARMSimulator;
using namespace std;

int main(void) {
  Cpu cpu(4096);

  cpu.MOV(Register::r0, 0x12345678);
  cpu.MOV(Register::r1, 0);
  cpu.MOV(Register::r9, 0xFF00);

  cpu.STM(Register::r1, true, Up, PostIndexed, {Register::r0, Register::r9});
  cpu.LDM(Register::r1, true, Down, PreIndexed, {Register::r2, Register::r3});

  cpu.dumpRegisters();
  return 0;
}
