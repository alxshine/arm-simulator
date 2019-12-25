#include "cpu.hpp"

#include <iostream>

using namespace std;

Cpu::Cpu(){
  for (int i = 0; i < mem_size; ++i) {
    mem[i] = 0;
  }
  for (int i = 0; i < 15; ++i) {
    r[i] = 0;
  }

}

void Cpu::dump_state(){
  cout << "Dumping CPU state" << endl;

  for (int i = 0; i < 16; ++i) {
    cout << "r" << i;
    if(i == 14)
      cout << "/lr";
    if(i == 15)
      cout << "/pc";

    cout << ":\t0x" << hex << r[i] << endl;
  }
}

void Cpu::mov(int rd, int r1){
  r[rd] = r[r1];
}

void Cpu::mov_immediate(int rd, unsigned int immediate){
  r[rd] = immediate;
}

void Cpu::add(int rd, int r1, int r2){
  r[rd] = r[r1] + r[r2];
}

void Cpu::add_immediate(int rd, int r1, unsigned int immediate){
  r[rd] = r[r1] + immediate;
}
