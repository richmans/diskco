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
#include <stdint.h>
#include <stdexcept>
#include "buffer_processor.h"
#include "progress.h"

class FileReader: public BufferProcessor {
private:
  int64_t _bytes_left;
  int64_t _cursor;
  int64_t _file_size;
  FILE* _file;
  Progress* _progress;
public:
  int process(Buffer* buffer);
  FileReader(Options* options);
  ~FileReader();
  void close();
  void initialize();
  
};
#endif /* defined(__Diskco__file_reader__) */
