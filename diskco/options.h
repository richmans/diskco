#ifndef OPTIONS_H
#define OPTIONS_H

#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <string>
#include <getopt.h>
#include <cstring>
#include "utils.h"

#ifndef fseeko
#define fseeko fseeko64
#define ftello ftello64
#endif

enum EntropyAlgorithm {none, nulls, logarithmic};

class Options {
private:
  std::string _input_filename;
  std::string _output_filename;
  int64_t _block_size;
  int64_t _processing_block_size;
  int64_t _offset;
  int64_t _length;
  int64_t _end;
  int64_t _sample_blocks;
  bool _swap_bytes;
  char* _search_bytes;
  int _search_bytes_length;
  EntropyAlgorithm _entropy_algorithm;
  bool _quiet;
  bool _append;
  int64_t _segment_offset;
  int64_t _segment_length;

  int64_t input_file_size();
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
  int search_bytes_length();
  int64_t block_size();
  int64_t processing_block_size();
  int64_t offset();
  int64_t length();
  int64_t sample_blocks();
  int64_t segment_offset();
  int64_t segment_length();
  EntropyAlgorithm entropy_algorithm();
  bool swap_bytes();
  bool quiet();
  bool append();

};
#endif
