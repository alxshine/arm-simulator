#include "barrel_shifter.hpp"
#include "constants.hpp"
#include "cpu.hpp"

#include <cstring>
#include <fstream>
#include <iostream>

using namespace ARMSimulator;
using namespace std;

int main(int argc, char **argv) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " ELF-Filename" << endl;
    return 1;
  }

  unsigned long memSize = 4 * 1024 * 1024;
  Cpu cpu(memSize);

  ifstream elfFile;
  elfFile.open(argv[1], ios::in | ios::binary);

  char buffer[1024];
  elfFile.read(buffer, ELF_HEADER_SIZE); // read entire file header
  if (memcmp(buffer, ELF_MAGIC_NUMBER, 4) != 0) {
    cerr << "ELF file " << argv[1] << " is not a valid ELF File" << endl;
    return 1;
  }

  if (buffer[4] != 1) {
    cerr << "ELF file " << argv[1]
         << " was compiled for 64bit, which is not supported" << endl;
    return 1;
  }

  if (buffer[0x13] != 0x28) {
    cerr << "ELF file " << argv[1] << " was not built for ARM" << endl;
    return 1;
  }

  unsigned int programEntry = 0;
  programEntry |= (buffer[0x18] & 0xFF) << 24;
  programEntry |= (buffer[0x19] & 0xFF) << 16;
  programEntry |= (buffer[0x1A] & 0xFF) << 8;
  programEntry |= buffer[0x1B] & 0xFF;

  cpu.setRegister(Register::pc, programEntry);

  unsigned int sectionHeaderStart = 0;
  sectionHeaderStart |= (buffer[0x20] & 0xFF) << 24;
  sectionHeaderStart |= (buffer[0x21] & 0xFF) << 16;
  sectionHeaderStart |= (buffer[0x22] & 0xFF) << 8;
  sectionHeaderStart |= buffer[0x23] & 0xFF;

  unsigned short sectionHeaderEntrySize = 0;
  sectionHeaderEntrySize |= (buffer[0x2E] & 0xFF) << 8;
  sectionHeaderEntrySize |= buffer[0x2F] & 0xFF;

  unsigned short numSectionHeaders = 0;
  numSectionHeaders |= (buffer[0x30] & 0xFF) << 8;
  numSectionHeaders |= buffer[0x31] & 0xFF;
  
  elfFile.seekg(sectionHeaderStart);

  cout << "Execution finished" << endl;
  return 0;
}
