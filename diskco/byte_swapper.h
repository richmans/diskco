//
//  byte_swapper.h
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#ifndef __Diskco__byte_swapper__
#define __Diskco__byte_swapper__

#include <stdio.h>
#include "buffer_processor.h"

class ByteSwapper: public BufferProcessor {
public:
  ByteSwapper(Options* options, BufferProcessor* parent, BufferPool* pool);
  Buffer* next_buffer();
};
#endif /* defined(__Diskco__byte_swapper__) */
