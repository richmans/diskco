//
//  BufferPool.cpp
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#include "buffer_pool.h"

BufferPool* BufferPool::m_pInstance = NULL;

BufferPool::BufferPool(int pool_size, int64_t buffer_size) {
  _pool = new Buffer*[pool_size];
  _pool_size = pool_size;
  _buffer_count = 0;
  _buffer_size = buffer_size;
}

Buffer* BufferPool::get_buffer(){
  Buffer* result;
  if (_buffer_count > 0) {
    _buffer_count -= 1;
    result = _pool[_buffer_count];
  } else {
    result = new Buffer(_buffer_size);
  }
  return result;
}

void BufferPool::release_buffer(Buffer* buffer){
  if (_buffer_count == _pool_size) {
    delete buffer;
  } else {
    _pool[_buffer_count] = buffer;
    _buffer_count += 1;
  }
}

