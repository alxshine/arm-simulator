#include "cpu.hpp"

int main(void)
{
  Cpu cpu;
  cpu.dump_state();

  cpu.mov_immediate(0, 128);
  cpu.add(1, 0, 0);
  cpu.dump_state();
  return 0;
}
