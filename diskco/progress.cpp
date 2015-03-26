//
//  progress.cpp
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#include "progress.h"
Progress::Progress(int64_t min, int64_t max){
  _min = min;
  _max = max;
  _cursor = min;
  _percent = 0;
  gettimeofday(&_last_progress, NULL);
  _interval = 0.5;
  _last_cursor = min;
  _last_speed = "0 b/s";
};

void Progress::set_status(std::string status){
  _status = status;
}

void Progress::update(int64_t cursor){
  _cursor = cursor;
  struct timeval current;
  gettimeofday(&current, NULL);
  double elapsed = ((double)current.tv_sec + (double)current.tv_usec / 1000000) - ((double)_last_progress.tv_sec + (double)_last_progress.tv_usec / 1000000);
  //printf("Elapsed %lld, inter %d\n", elapsed, _interval);
  if (elapsed > _interval) print();
};

void Progress::print(){
  int64_t processed = _cursor - _last_cursor;
  _last_cursor = _cursor;
  _percent = int((float(_cursor - _min) / float(_max - _min)) * 100);
  struct timeval current;
  gettimeofday(&current, NULL);
  double elapsed = ((double)current.tv_sec + (double)current.tv_usec / 1000000) - ((double)_last_progress.tv_sec + (double)_last_progress.tv_usec / 1000000);
  
  gettimeofday(&_last_progress, NULL);
  //printf("Clock %lf, cursor %lld\n", elapsed, processed);
  //printf("Elapsed %d\n", elapsed);
  int64_t speed = double(processed) / (double(elapsed));
  std::string speed_pretty = pretty_bytes(speed);
  printf("\r%s %d%% complete. %s/s     ", _status.c_str(), _percent, speed_pretty.c_str());
  _last_speed = speed_pretty;
  fflush(stdout);
};

void Progress::finish() {
  printf("\r100%% complete. %s/s            \n", _last_speed.c_str());
}
