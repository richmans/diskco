#ifndef OPTIONS_H
#define OPTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <getopt.h>
#include "utils.h"
class Options {
private:
  std::string _input_filename;
  std::string _output_filename;
  int64_t _block_size;
  int64_t _offset;
  int64_t _length;
  int64_t _end;
  bool _swap_bytes;
  bool _quiet;
  bool _append;
public:
  Options(int argc, char* argv[]);
  Options(char* input, char* output, bool append, bool byteswap);
  void set_length(int64_t length);
  void set_offset(int64_t offset);
  void set_swap_bytes(bool swap_bytes);
  std::string check_arguments();
  std::string input_filename();
  std::string output_filename();
  int64_t block_size();
  int64_t offset();
  int64_t length();
  bool swap_bytes();
  bool quiet();
  bool append();
  
};
#endif