//
//  file_writer.h
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#ifndef __Diskco__file_writer__
#define __Diskco__file_writer__

#include <stdio.h>
#include <stdexcept>
#include <stdint.h>
#include "buffer_processor.h"
class FileWriter: public BufferProcessor {
private:
  FILE* _file;
public:
  FileWriter(Options* options, BufferProcessor* parent, BufferPool* pool);
  ~FileWriter();
  void close();
  Buffer* next_buffer();
};
#endif /* defined(__Diskco__file_writer__) */
