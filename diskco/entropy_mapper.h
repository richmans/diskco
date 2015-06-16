//
//  file_reader.h
//  Diskco
//
//  Created by Martin van Wingerden on 12/05/15.
//  Copyright (c) 2015 Martin van Wingerden All rights reserved.
//

#ifndef __Diskco__entropy_mapper__
#define __Diskco__entropy_mapper__

#include <stdio.h>
#include <string>
#include <stdexcept>
#include <stdint.h>
#include "buffer_processor.h"
#include "file_reader.h"
class EntropyMapper: public BufferProcessor {
private:
  int64_t _entropy_cursor;
  Buffer* _current_input_buffer;
  Buffer* _current_output_buffer;
  int64_t _current_input_size;
  bool _header_written;
  int _output_cursor_step;
  double _entropy;
  int _zeroes;
  int* _histo_map;

  void fetch_next_buffer();
  bool will_an_extra_input_buffer_fit();
  void process_input_buffer();
  Buffer* write_header();
  void process_char(char input);
  void process_entropy();
  int calculate_entropy();
public:
  Buffer* next_buffer();
  void close();
  int64_t cursor();
  EntropyMapper(Options* options, BufferProcessor* parent, BufferPool* pool);
  ~EntropyMapper();
  void initialize();

};
#endif /* defined(__Diskco__entropy_mapper__) */
