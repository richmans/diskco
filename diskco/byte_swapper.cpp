//
//  byte_swapper.cpp
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#include "byte_swapper.h"
ByteSwapper::ByteSwapper(Options* options) : BufferProcessor(options) {
}

int ByteSwapper::process(Buffer *buffer) {
  char* data = buffer->buffer();
  char temp;
  for (int i=0; i < (buffer->size() -1) ; i+=2) {
    temp = data[i];
    data[i] = data[i+1];
    data[i+1] = temp;
  }
  return 0;
}