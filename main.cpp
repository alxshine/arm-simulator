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
  cpu.MOV(Register::r9, 0xFF);

  cpu.STR(Register::r0, Register::r1, 4, true, Word, Up, PreIndexed);
  cpu.STR(Register::r9, Register::r1, -2, false, Byte, Up, PreIndexed);
  cpu.LDR(Register::r2, Register::r1, 2, true, Word, Down, PostIndexed,
          {ShiftType::LogicalLeft, 1});
  cpu.LDR(Register::r3, Register::r1, 2, false, Byte, Up, PreIndexed);
  cpu.LDR(Register::r4, Register::r1, 0, false, Word, Up, PreIndexed);

  cpu.dumpRegisters();
  return 0;
}
