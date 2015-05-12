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
#include <stdint.h>
#include <string>
#include <stdexcept>
#include "buffer_processor.h"
#include "file_reader.h"
class EntropyMapper: public BufferProcessor {
private:
  int64_t _entropy_cursor;
  Buffer* _current_input_buffer;
  Buffer* _current_output_buffer;
  int64_t _current_input_size;
  bool _header_written;
  uint32_t _entropy;
  uint32_t _bit_index;

  void fetch_next_buffer();
  void process_input_buffer();
  Buffer* write_header();

  bool process_char(char input);
public:
  Buffer* next_buffer();
  void close();
  int64_t cursor();
  EntropyMapper(Options* options, BufferProcessor* parent, BufferPool* pool);
  ~EntropyMapper();
  void initialize();

};
#endif /* defined(__Diskco__entropy_mapper__) */
