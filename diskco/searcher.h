//
//  file_reader.h
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#ifndef __Diskco__searcher__
#define __Diskco__searcher__

#include <stdio.h>
#include <stdint.h>
#include <stdexcept>
#include "buffer_processor.h"

class Searcher: public BufferProcessor {
private:
  FILE* _file;
  int _match_cursor;
  int _head_size;
  const char* _match_bytes;
  int _match_size;
public:
  int process(Buffer* buffer);
  void close();
  Searcher(Options* options);
  ~Searcher();
  void initialize();
  
};
#endif /* defined(__Diskco__searcher__) */
