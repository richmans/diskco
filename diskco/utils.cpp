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

char* hextobytes(const char* hex){
  char* _search_bytes = new char[strlen(hex) / 2];
  char* buffer = new char[3];
  buffer[2] = 0;
  int length = strlen(hex);
  for (int i = 0; i < length - 1; i += 2) {
    buffer[0] = hex[i];
    buffer[1] = hex[i + 1];
    _search_bytes[i/2] = (char)strtoul(buffer, NULL, 16);
  }

  return _search_bytes;
}

std::string pretty_bytes(int64_t bytes) {
  char result[30];
  std::string unit = " B";
  if (bytes >= 1024) {
    unit = "KB";
    bytes = bytes >> 10;
  }
  if (bytes >= 1024) {
    unit = "MB";
    bytes = bytes >> 10;
  }
  if (bytes >= 1024) {
    unit = "GB";
    bytes = bytes >> 10;
  }
  if (bytes >= 1024) {
    unit = "TB";
    bytes = bytes >> 10;
  }
  sprintf(result, "%lld %s", bytes, unit.c_str());
  return result;
}
