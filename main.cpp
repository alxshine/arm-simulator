#include "barrel_shifter.hpp"
#include "constants.hpp"
#include "cpu.hpp"

#include <cstring>
#include <fstream>
#include <iostream>

using namespace ARMSimulator;
using namespace std;

int readFromElf(const char *buffer, const unsigned int start,
                const unsigned int size) {
  int ret = 0;
  for (unsigned int i = 0; i < size; ++i)
    ret |= (buffer[start + i] & 0xFF) << (size - 1 - i) * 8;

  return ret;
}

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

  if (!(buffer[0x13] == 0x28 || buffer[0x12] == 0x28)) {
    cerr << "ELF file " << argv[1] << " was not built for ARM" << endl;
    return 1;
  }

  unsigned int programEntry = readFromElf(buffer, 0x18, 4);
  cpu.setRegister(Register::pc, programEntry);
  cpu.setRegister(Register::sp, 0x10000);

  unsigned int sectionHeaderStart = readFromElf(buffer, 0x20, 4);
  unsigned short sectionHeaderEntrySize = readFromElf(buffer, 0x2E, 2);
  unsigned short numSectionHeaders = readFromElf(buffer, 0x30, 2);

  elfFile.seekg(sectionHeaderStart);

  for (int i = 0; i < numSectionHeaders; ++i) {
    elfFile.read(buffer, sectionHeaderEntrySize);
    int sectionType = readFromElf(buffer, 0x04, 4);
    if (sectionType != 0x01)
      continue;

    int sectionAddress = readFromElf(buffer, 0x0c, 4);
    int sectionOffset = readFromElf(buffer, 0x10, 4);
    int sectionSize = readFromElf(buffer, 0x14, 4);

    auto currentPosition = elfFile.tellg();

    // load section to memory
    elfFile.seekg(sectionOffset);
    char sectionBuffer[sectionSize];
    elfFile.read(sectionBuffer, sectionSize);
    cpu.setMemory(sectionAddress, (unsigned char *)sectionBuffer, sectionSize);

    elfFile.seekg(currentPosition);
  }
  elfFile.close();

  try {
    while (true)
      cpu.nextInstruction();
  } catch (int returnCode) {
    cout << "Internal program exited with code " << returnCode << endl;
  } catch (std::logic_error &e){
    const char *message = e.what();
    cout << "Internal error:" << endl << message << endl;
  }

  cout << "Execution finished" << endl;
  return 0;
}
