#include "barrel_shifter.hpp"
#include "constants.hpp"
#include "cpu.hpp"

#include <iostream>

using namespace ARMSimulator;
using namespace std;

int main(void) {
  Cpu cpu(4 * 1024 * 1024);

  char data[] = "Hello, World!\n";
  int len = 16;
  unsigned int address = 0x2009c;
  cpu.setMemory(address, (unsigned char *)data, len);
  cpu.setMemoryWord(0x10098, 0x2009c);
  cpu.setRegister(Register::pc, 0x10074);

  try {
    // cpu.dumpRegisters();
    cpu.executeInstruction(0xe3a00000);
    cpu.executeInstruction(0xe59f1014);
    cpu.executeInstruction(0xe3a0200e);
    cpu.executeInstruction(0xe3a07004);
    cpu.executeInstruction(0xef000000);
    // cpu.dumpRegisters();

    cpu.executeInstruction(0xe3a00000);
    cpu.executeInstruction(0xe3a07001);
    cpu.executeInstruction(0xef000000);
    // cpu.dumpRegisters();
  } catch (int returnCode) {
    cout << "Internal program exited with returncode " << returnCode << endl;
  }

  cout << "Execution finished" << endl;
  return 0;
}
