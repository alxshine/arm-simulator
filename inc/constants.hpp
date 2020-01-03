#pragma once

namespace ARMSimulator {
const unsigned int FLAG_MASK = 0xe0000000;

enum Conditions {
  EQ = 0,
  NE,
  CS,
  CC,
  MI,
  PL,
  VS,
  VC,
  HI,
  LS,
  GE,
  LT,
  GT,
  LE,
  AL
};
} // namespace ARMSimulator
