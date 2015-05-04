//
//  file_reader.h
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#ifndef __Diskco__searcher__
#define __Diskco__searcher__

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <stdexcept>
#include "buffer_processor.h"
#include "file_reader.h"
class Searcher: public BufferProcessor {
private:
  int64_t _search_cursor;
  Buffer* _current_input_buffer;
  FileReader* _current_output_reader;
  int64_t _current_input_size;
  bool _reading_result;
  int _match_cursor;
  int* _failure_map;

  char* _match_bytes;
  int _match_size;
  bool process_char(char input);
  void fetch_next_buffer();
  bool search_match();
  bool search_match_in_buffer();
  void setup_output_reader(int64_t offset);
  void failure_function(int size);
public:
  Buffer* next_buffer();
  void close();
  int64_t cursor();
  Searcher(Options* options, BufferProcessor* parent, BufferPool* pool);
  ~Searcher();
  void initialize();

};
#endif /* defined(__Diskco__searcher__) */
