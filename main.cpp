#include "barrel_shifter.hpp"
#include "cpu.hpp"
#include "type_definitions.hpp"

#include <iostream>

using namespace ARMSimulator;
using namespace std;

int main(void) {
  Cpu cpu;
  cpu.dumpRegisters();

  cpu.setRegister(lr, 0x11);
  cout << hex << "0x" << cpu.getRegister(lr) << endl;

  BarrelShifterConfig config = {ShiftType::RotateRight, 2};
  auto shiftResult = BarrelShifter::executeConfig(cpu.getRegister(lr), config);
  cout << "Result: 0x" << shiftResult.value << ", carry: " << shiftResult.carry
       << endl;
  return 0;
}
