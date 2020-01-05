#pragma once

#include <stdexcept>
#include <string>

class NotImplementedException : public std::logic_error {
public:
  NotImplementedException(const std::string functionName)
      : std::logic_error("Function " + functionName + " not yet implemented"){};
};

class InvalidMemoryAccessException : public std::logic_error {
public:
  InvalidMemoryAccessException(const unsigned int address)
      : std::logic_error("Error trying to access memory at address " +
                         std::to_string(address)) {}
};

class InvalidRegisterException : public std::logic_error {
public:
  InvalidRegisterException(const unsigned int intReg)
      : std::logic_error("Error trying to access register " +
                         std::to_string(intReg)) {}
};

class IllegalStateException : public std::logic_error {
public:
  IllegalStateException(const std::string errorText)
      : std::logic_error(errorText) {}
};
