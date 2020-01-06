#include "exceptions.hpp"

#include "sstream"

ARMSimulator::InvalidMemoryAccessException::InvalidMemoryAccessException(
    const unsigned int address)
    : std::logic_error("Use a reference to get the actual message"), address{address} {
  std::stringstream output;
  output << "Invalid memory access at address " << std::hex << std::showbase
         << address;

  message = output.str();
}

const char *ARMSimulator::InvalidMemoryAccessException::what() const noexcept {
  return message.c_str();
}
