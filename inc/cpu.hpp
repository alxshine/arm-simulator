#pragma once

class Cpu{
private:
  static const long mem_size = 4096;
  
  int registers[15];
  unsigned char memory[mem_size];
  unsigned char cspr;

public:
  Cpu();
  void dump_state();
};
