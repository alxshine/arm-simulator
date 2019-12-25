#include "cpu.hpp"

#include <iostream>

using namespace std;

Cpu::Cpu(){
  for (int i = 0; i < mem_size; ++i) {
    memory[i] = 0;
  }
}

void Cpu::dump_state(){
  cout << "Hello, this is dump_state" << endl;
}
