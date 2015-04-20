#ifndef OPTIONS_H
#define OPTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
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
  char* _search_bytes;
  bool _quiet;
  bool _append;
  int64_t _segment_offset;
  int64_t _segment_length;
public:
  Options(int argc, char* argv[]);
  Options(char* input, char* output, bool append, bool byteswap);
  void set_length(int64_t length);
  void set_offset(int64_t offset);
  void set_swap_bytes(bool swap_bytes);
  void set_search_bytes(std::string search_bytes);
  void set_segment_offset(int64_t segment_offset);
  void set_segment_length(int64_t segment_offset);
  std::string check_arguments();
  std::string input_filename();
  std::string output_filename();
  char* search_bytes();
  int64_t block_size();
  int64_t offset();
  int64_t length();
  int64_t segment_offset();
  int64_t segment_length();
  bool swap_bytes();
  bool quiet();
  bool append();

};
#endif
