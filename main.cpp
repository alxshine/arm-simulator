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

  cpu.STR(Register::r0, Register::r1, 4, true, Word, Up, PreIndexed);
  // cpu.setMemoryWord(0, cpu.getRegister(Register::r0));
  cout << hex << (int)cpu.getMemoryWord(4) << endl;

  cpu.dumpRegisters();
  return 0;
}
