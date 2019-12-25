#include "cpu.hpp"
#include <iostream>

using namespace ARMSimulator;
using namespace std;

int main(void)
{
  Cpu cpu;
  cpu.dumpRegisters();

  cpu.setRegister(lr, 0x12233);
  cout << hex << "0x" << cpu.getRegister(lr) << endl;
  return 0;
}
