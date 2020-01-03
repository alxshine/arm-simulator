#include "barrel_shifter.hpp"
#include "cpu.hpp"
#include "type_definitions.hpp"

#include <iostream>

using namespace ARMSimulator;
using namespace std;

int main(void) {
  Cpu cpu(4 * 1024 * 1024);

  char data[] = "Hello, World!\n";
  int len = 16;
  unsigned int address = 0x20098;
  cpu.setMemory(address, (unsigned char *)data, len);

  try {
    cout << cpu.getRegister(Register::pc) << endl;
    cpu.nextInstruction(0xe3a0200e);
  } catch (int returnCode) {
    cout << "Internal program exited with returncode " << returnCode << endl;
  }
  cout << "Execution finished" << endl;
  return 0;
}
