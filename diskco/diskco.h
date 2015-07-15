#ifndef DISCO_H
#define DISCO_H
#include <stdio.h>
#include <stdexcept>
#include <string.h>
#include <stdint.h>
#include "options.h"
#include "buffer.h"
#include "file_reader.h"
#include "file_writer.h"
#include "byte_swapper.h"
#include "entropy_mapper.h"
#include "buffer_pool.h"
#include "searcher.h"

class Diskco {
private:
  void get_options(int argc, char* argv[]);
  Options *_options;
  FileReader *_reader;
  ByteSwapper *_swapper;
  Searcher *_searcher;
  EntropyMapper *_entropy;
  FileWriter *_writer;
  BufferPool* _pool;
  BufferProcessor* _chain_end;
  void run();
  void initialize();
  void rewire();
public:
  ~Diskco();
  Diskco(int argc, char* argv[]);
  Diskco(char* input, char* output, bool append, bool byteswap);
  void help();
  void set_swap_bytes(bool swap_bytes);
  void set_search(std::string search_bytes, int64_t offset, int64_t length, int64_t segment_offset, int64_t segment_length);
  void unset_search();
  void copy(int64_t offset, int64_t length);
  Buffer* next_buffer();
  void release_buffer(Buffer* buffer);
  void close();
  void start();

};
#endif
