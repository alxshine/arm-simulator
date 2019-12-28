#pragma once

#include <stdexcept>
#include <string>

class NotImplementedException : public std::logic_error {
 public:
  NotImplementedException(const std::string functionName)
      : std::logic_error("Function " + functionName + " not yet implemented"){};
};