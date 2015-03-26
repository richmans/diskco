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
#include "buffer.h"
class BufferProcessor {
protected:
  Options* _options;
public:
  BufferProcessor(Options* options);
  int process(Buffer* buffer);
  void close();
};

#endif /* defined(__Diskco__BufferProcessor__) */
