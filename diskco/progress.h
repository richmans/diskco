//
//  progress.h
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#ifndef __Diskco__progress__
#define __Diskco__progress__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/time.h>
#include "utils.h"
class Progress{
private:
  int64_t _cursor;
  int64_t _last_cursor;
  int64_t _min;
  int64_t _max;
  struct timeval _last_progress;
  int _percent;
  float _interval;
  std::string _last_speed;
  std::string _status;
public:
  Progress(int64_t min, int64_t max);
  void set_status(std::string status);
  void update(int64_t cursor);
  void print();
  void finish();
};

#endif /* defined(__Diskco__progress__) */
