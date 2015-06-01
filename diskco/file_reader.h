//
//  file_reader.h
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#ifndef __Diskco__file_reader__
#define __Diskco__file_reader__

#include <stdio.h>
#include <stdexcept>
#include <stdint.h>
#include "buffer_processor.h"
#include "progress.h"

class FileReader: public BufferProcessor {
private:
  int64_t _bytes_left;
  int64_t _cursor;
  FILE* _file;
  Progress* _progress;
public:
  Buffer* next_buffer();
  FileReader(Options* options, BufferProcessor* parent, BufferPool* pool);
  ~FileReader();
  int64_t cursor();
  void close();
  void initialize();
  void initialize(int64_t offset, int64_t length);
};
#endif /* defined(__Diskco__file_reader__) */
