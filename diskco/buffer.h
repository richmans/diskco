//
//  buffer.h
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#ifndef __Diskco__buffer__
#define __Diskco__buffer__
#include <stdio.h>
#include <stdlib.h>

class Buffer {
private:
  char* _buffer;
  int64_t _size;
  int64_t _capacity;
  int64_t _source_offset;
public:
  Buffer(int64_t capacity);
  ~Buffer();
  void set_size(int64_t size);
  void set_source_offset(int64_t source_offset);
  int64_t source_offset();
  int64_t size();
  int64_t capacity();
  char* buffer();
 
};
#endif /* defined(__Diskco__buffer__) */
