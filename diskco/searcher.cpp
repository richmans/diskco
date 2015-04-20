//
//  searcher.cpp
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#include "searcher.h"
Searcher::Searcher(Options* options, BufferProcessor* parent, BufferPool* pool) : BufferProcessor(options, parent, pool) {
  //TODO Convert from hex and calculate _head_size                     
  _match_bytes = options->search_bytes().c_str(); 
  _current_output_reader = new FileReader(_options, NULL, _pool); 
  initialize();
};

void Searcher::close() {
  if(_current_input_buffer != NULL) _pool->release_buffer(_current_input_buffer);
  if(_current_output_reader != NULL) {
    delete _current_output_reader;
  }
}

void Searcher::initialize() {
  _match_cursor = 0;
  _search_cursor = 0;
  _current_input_buffer = NULL;
  _current_input_size = 0;
  _current_output_reader = NULL;
  _reading_result = false;
}

void Searcher::fetch_next_buffer() {
  if(_current_input_buffer != NULL) _pool->release_buffer(_current_input_buffer);
  _current_input_buffer = _parent->next_buffer();
  _search_cursor = int64_t(0);
  if (_current_input_buffer != NULL) {
    _current_input_size = _current_input_buffer->size();
  }
}

Searcher::~Searcher() {
  this->close();
}

void Searcher::setup_output_reader() {
  int64_t offset = _search_cursor + _options->segment_offset();
  _current_output_reader->initialize(offset, _options->segment_length());
  _reading_result = true;
}

bool Searcher::process_char(char input){
  if(_match_bytes[_match_cursor] == input) {
    _match_cursor += 1;
    if(_match_size == _match_cursor){
      _match_cursor = 0;
      setup_output_reader();
      return true;
    }
  } else if (_match_cursor > 0 && _match_cursor == _head_size && input  == _match_bytes[_match_cursor -1]) {
    // sliding window
  }else{
    _match_cursor = 0;
  }
  return false;
}

bool Searcher::search_match_in_buffer(){
  bool match_found;
  char* data = _current_input_buffer->buffer();
  while(_current_input_buffer != NULL && _search_cursor < _current_input_size) {
    match_found = process_char(data[_search_cursor]);
    if(match_found) return true;
    _search_cursor += 1;
  }
  return false;
}

bool Searcher::search_match(){
  bool match_found = false;
  if(_current_input_buffer == NULL) fetch_next_buffer();
  while(_current_input_buffer != NULL) { 
    match_found = search_match_in_buffer();
    if(match_found) return true;
    fetch_next_buffer();
  }
  return false;
}

Buffer* Searcher::next_buffer() {
  Buffer* buffer;
  if (_current_output_reader != NULL) {
    buffer = _current_output_reader->next_buffer();
    if(buffer != NULL) return buffer;
    _reading_result = false;
  }
  bool match_found = search_match();
  if (!match_found) return NULL;
  return _current_output_reader->next_buffer();
}