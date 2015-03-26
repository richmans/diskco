//
//  BufferProcessor.cpp
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#include "buffer_processor.h"
BufferProcessor::BufferProcessor(Options* options){
  _options = options;
}

void BufferProcessor::close(){
  
}

// does work on a buffer
// returns 0 if succesful, -1 if further processing should be stopped.
int BufferProcessor::process(Buffer* buffer){
  return 0;
}
