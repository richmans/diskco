#ifndef DISCO_H
#define DISCO_H
#include <stdio.h>
#include <stdint.h>
#include <stdexcept>
#include <string.h>
#include "options.h"
#include "buffer.h"
#include "file_reader.h"
#include "file_writer.h"
#include "byte_swapper.h"
#include "searcher.h"
class Diskco {
private:
  void get_options(int argc, char* argv[]);
  Options *_options;
  Buffer *_buffer;
  FileReader *_reader;
  ByteSwapper *_swapper;
  FileWriter *_writer;
  BufferPool* _pool;
  void run();
  void initialize();
public:
  ~Diskco();
  Diskco(int argc, char* argv[]);
  Diskco(char* input, char* output, bool append, bool byteswap);
  void help();
  void set_swap_bytes(bool swap_bytes);
  void copy(int64_t offset, int64_t length);
  void close();
  void start();

};
#endif
