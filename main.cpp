#include "cpu.hpp"
#include "barrel_shifter.hpp"
#include "type_definitions.hpp"

#include <iostream>

using namespace ARMSimulator;
using namespace std;

int main(void)
{
  Cpu cpu;
  cpu.dumpRegisters();

  cpu.setRegister(lr, 0x11);
  cout << hex << "0x" << cpu.getRegister(lr) << endl;

  auto shiftResult = BarrelShifter::rotateRight(cpu.getRegister(lr), 1);
  cout << "Result: 0x" << shiftResult.value << ", carry: " << shiftResult.carry << endl;
  return 0;
}
