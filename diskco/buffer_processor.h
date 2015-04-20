//
//  BufferProcessor.h
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#ifndef __Diskco__BufferProcessor__
#define __Diskco__BufferProcessor__

#include <stdio.h>
#include "options.h"
#include "buffer_pool.h"
#include "buffer.h"
class BufferProcessor {
protected:
  Options* _options;
  BufferPool* _pool;
  BufferProcessor* _parent;
public:
  BufferProcessor(Options* options, BufferProcessor* parent, BufferPool* _pool);
  virtual Buffer* next_buffer();
  virtual void close();
  void set_parent(BufferProcessor* parent);
};

#endif /* defined(__Diskco__BufferProcessor__) */
