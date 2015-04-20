#include "utils.h"

int64_t bytesize(std::string input) {
  char multiplier;
  int64_t number;
  sscanf(input.c_str(), "%lld%c", &number, &multiplier);
  switch(multiplier) {
    case 'k':
    case 'K':
      number *= 1024;
      break;
    case 'm':
    case 'M':
      number *= 1024 * 1024;
      break;
    case 'g':
    case 'G':
      number *= 1024 * 1024 * 1024;
      break;
    case 't':
    case 'T':
      number *= int64_t(1024) * 1024 * 1024 * 1024;
      break;
  }
  return number;
}

std::string pretty_bytes(int64_t bytes) {
  char result[30];
  std::string unit = " b";
  if (bytes >= 1024) {
    unit = "Kb";
    bytes = bytes >> 10;
  }
  if (bytes >= 1024) {
    unit = "Mb";
    bytes = bytes >> 10;
  }
  if (bytes >= 1024) {
    unit = "Gb";
    bytes = bytes >> 10;
  }
  if (bytes >= 1024) {
    unit = "Tb";
    bytes = bytes >> 10;
  }
  sprintf(result, "%lld %s", bytes, unit.c_str());
  return result;
}
