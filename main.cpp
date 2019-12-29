#include "barrel_shifter.hpp"
#include "cpu.hpp"
#include "type_definitions.hpp"

#include <iostream>

using namespace ARMSimulator;
using namespace std;

int main(void) {
  Cpu cpu(4096);

  cpu.setRegister(Register::lr, 0x11);
  cpu.MOV(Register::r0, 0x80000000);
  cpu.AND(Register::r1, Register::r0, Register::r0, {ShiftType::LogicalLeft, 0},
          true);

  cpu.dumpRegisters();

  cpu.setMemoryByte(0, 0x12);
  cpu.setMemoryByte(1, 0x34);
  cpu.setMemoryByte(2, 0x56);
  cpu.setMemoryByte(3, 0x78);
  cout << hex << cpu.getMemoryWord(0) << endl;

  cpu.setMemoryWord(0, 0x87654321);
  cout << hex << (int)cpu.getMemoryByte(1) << endl;
  cpu.getMemoryByte(50000);

  return 0;
}
