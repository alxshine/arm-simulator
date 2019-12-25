#pragma once

class Cpu{
private:
  static const long mem_size = 4096;
  
  int r[16];
  unsigned char mem[mem_size];
  unsigned char cspr;

public:
  Cpu();
  void dump_state();

  void mov(int rd, int r1);
  void mov_immediate(int rd, unsigned int immediate);
  void add(int rd, int r1, int r2);
  void add_immediate(int rd, int r1, unsigned int immediate);
};
