//
//  searcher.cpp
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#include "searcher.h"
Searcher::Searcher(Options* options) : BufferProcessor(options) {
  _file = fopen(options->input_filename().c_str(), "rb");
  //TODO Convert from hex and calculate _head_size
  _match_bytes = options->search_bytes().c_str(); 
  if(!_file) throw std::runtime_error("Could not open input file");
  initialize();
};

void Searcher::close() {
  if(_file != NULL) {
    fclose(_file);
    _file = NULL;
  }
}

void Searcher::initialize() {
  _match_cursor = 0;
}

Searcher::~Searcher() {
  this->close();
  delete _file;
}


void Searcher::process_char(char input){
  if(_match_bytes[_match_cursor] == input) {
    _match_cursor += 1;
    if(_match_size == _match_cursor){
      _match_cursor = 0;
      process_match();
    }
  } else if (_match_cursor > 0 && _match_cursor == _head_size && input  == _match_bytes[_match_cursor -1])
    // sliding window
  }else{
    _match_cursor = 0;
  }
}

int Searcher::process(Buffer *buffer) {
  char* data = buffer->buffer();
  for(int i =0; i<buffer->size(); i++){
    process_char(buffer[i]);
  }
}