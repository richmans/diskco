//
//  mock_buffer_processor.cpp
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#include "mock_buffer_processor.h"
MockBufferProcessor::MockBufferProcessor(Options* options, BufferPool* pool, int buffer_generator) : BufferProcessor(options, NULL, pool) {
  _buffer_generator = buffer_generator;
}

Buffer* MockBufferProcessor::next_buffer() {
  Buffer* buffer;
  
  if (_buffer_generator == 0) {
    buffer = _pool->get_buffer();
  } else {
    buffer = _instant_buffer;
  }
  
  return buffer;
}

void MockBufferProcessor::set_next_buffer(Buffer* buffer){
  _instant_buffer = buffer;
  _buffer_generator = 1;
}