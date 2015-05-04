//
//  searcher.cpp
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#include "searcher.h"
Searcher::Searcher(Options* options, BufferProcessor* parent, BufferPool* pool) : BufferProcessor(options, parent, pool) {
  initialize();
};

void Searcher::close() {
  if(_current_input_buffer != NULL) _pool->release_buffer(_current_input_buffer);
  if(_current_output_reader != NULL) {
    delete _current_output_reader;
  }
}

int64_t Searcher::cursor() {
  int64_t result = _search_cursor;
  if(_current_input_buffer != NULL) result += _current_input_buffer->source_offset();
  return result;
}

void Searcher::initialize() {
  _match_cursor = 0;
  _search_cursor = 0;
  if(_parent != NULL) _search_cursor = _parent->cursor();
  _current_input_buffer = NULL;
  _current_input_size = 0;
  _current_output_reader = NULL;
  _reading_result = false;

  _match_size = _options->search_bytes_length();
  _match_bytes = new char[_match_size];

  if (_options->search_bytes_length() > 0) {
    memcpy(_match_bytes, _options->search_bytes(), _options->search_bytes_length());
    // implements the KMP string searching algorithm
    failure_function(_match_size);
  }
}

void Searcher::failure_function(int size) {
  _failure_map = new int[size];
  int i = 1;
  int j = 0;
  _failure_map[0] = 0;
  while (i < size) {
    if (_match_bytes[i] == _match_bytes[j]) {
      // we have matched j + 1 characters
      _failure_map[i] = j + 1;
      i++;
      j++;
    } else if (j > 0) {
      // j indexes just after a prefix that matches
      j = _failure_map[j - 1];
    } else {
      _failure_map[i] = 0;
      i++;
    }
  }
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

void Searcher::setup_output_reader(int64_t offset) {
  if(_current_output_reader == NULL) _current_output_reader = new FileReader(_options, NULL, _pool);
  _current_output_reader->initialize(offset, _options->segment_length());
  _reading_result = true;
}

bool Searcher::process_char(char input){
  if(_match_bytes[_match_cursor] == input) {
    _match_cursor += 1;
    if(_match_size == _match_cursor){
      _match_cursor = 0;
      int64_t offset = cursor() - _match_size + _options->segment_offset() + 1;

      setup_output_reader(offset);

      if (!_options->quiet()) printf("\rFound a match at: %lld\n", offset);

      return true;
    }
  } else if (_match_cursor > 1) {
    //printf("Mismatch, reseting match cursor, updated from %d to %d on char: %c\n ", _match_cursor, _failure_map[_match_cursor - 1], input);
    _match_cursor = _failure_map[_match_cursor - 1];
    // reprocess the current position
    return process_char(input);
  } else {
    _match_cursor = 0;
  }
  return false;
}

bool Searcher::search_match_in_buffer(){
  bool match_found;
  char* data = _current_input_buffer->buffer();
  while(_current_input_buffer != NULL && _search_cursor < _current_input_size) {
    match_found = process_char(data[_search_cursor]);
    _search_cursor += 1;
    if(match_found) break;
  }
  return match_found;
}

bool Searcher::search_match(){
  bool match_found = false;
  if(_current_input_buffer == NULL) fetch_next_buffer();
  while(_current_input_buffer != NULL) {
    match_found = search_match_in_buffer();
    if(match_found) break;
    fetch_next_buffer();
  }
  return match_found;
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
