//
//  byte_swapper.cpp
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#include "byte_swapper.h"
ByteSwapper::ByteSwapper(Options* options, BufferProcessor* parent, BufferPool* pool) : BufferProcessor(options, parent, pool) {
}

Buffer* ByteSwapper::next_buffer() {
  Buffer* buffer = _parent->next_buffer();
  if(buffer == NULL) return NULL;
  char* data = buffer->buffer();
  char temp;
  for (int i=0; i < (buffer->size() -1) ; i+=2) {
    temp = data[i];
    data[i] = data[i+1];
    data[i+1] = temp;
  }
  return buffer;
}