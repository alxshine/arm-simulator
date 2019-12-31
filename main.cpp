#include "barrel_shifter.hpp"
#include "cpu.hpp"
#include "type_definitions.hpp"

#include <iostream>

using namespace ARMSimulator;
using namespace std;

int main(void) {
  Cpu cpu(4096);

  char data[] = "Hello, World!\n";
  int len = 16;
  unsigned int address = 0x100;
  cpu.setMemory(address, (unsigned char *)data, len);

  cpu.MOV(Register::r0, 0);
  cpu.MOV(Register::r1, address);
  cpu.MOV(Register::r2, len);
  cpu.MOV(Register::r7, 4);
  cpu.SWI();

  cpu.MOV(Register::r0, 0);
  cpu.MOV(Register::r7, 1);
  cpu.SWI();
  return 0;
}
