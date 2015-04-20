//
//  BufferProcessor.cpp
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#include "buffer_processor.h"
BufferProcessor::BufferProcessor(Options* options, BufferProcessor* parent, BufferPool* pool){
  _options = options;
  _parent = parent;
  _pool = pool;
}

void BufferProcessor::close(){
  
}

// returns the next output buffer, calls parent.next_buffer if necessary
// returns NULL if no more data is available
Buffer* BufferProcessor::next_buffer(){
  printf("HALLO\n");
  return NULL;
}

void BufferProcessor::set_parent(BufferProcessor* parent) {
  _parent = parent;
}