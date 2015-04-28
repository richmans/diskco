//
//  mock_buffer_processor.h
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#ifndef __Diskco__mock_buffer_processor__
#define __Diskco__mock_buffer_processor__

#include <stdio.h>
#include "buffer.h"
#include "buffer_processor.h"
#include "buffer_pool.h"

class MockBufferProcessor: public BufferProcessor {
private:
  int _buffer_generator;
  Buffer* _instant_buffer;
public:
  MockBufferProcessor(Options* options, BufferPool* pool, int buffer_generator);
  Buffer* next_buffer();
  void set_next_buffer(Buffer* buffer);
  
};
#endif /* defined(__Diskco__mock_buffer_processor__) */
