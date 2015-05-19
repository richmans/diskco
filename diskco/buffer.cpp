//
//  buffer.cpp
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#include "buffer.h"
Buffer::Buffer(int64_t capacity){
  _capacity = capacity;
  _size = 0;
  _buffer = (char*)malloc(capacity);
}

Buffer::~Buffer(){
  free(_buffer);
}

int64_t Buffer::size(){
  return _size;
}

void Buffer::set_size(int64_t size){
  _size = size;
}

int64_t Buffer::capacity(){
  return _capacity;
}

char* Buffer::buffer() {
  return _buffer;
}

int64_t Buffer::source_offset() {
  return _source_offset;
}

void Buffer::set_source_offset(int64_t source_offset){
  _source_offset = source_offset;
}
