//
//  searcher.cpp
//  Diskco
//
//  Created by Richard Bronkhorst on 12/05/15.
//  Copyright (c) 2015 Martin van Wingerden All rights reserved.
//

#include "entropy_mapper.h"
EntropyMapper::EntropyMapper(Options* options, BufferProcessor* parent, BufferPool* pool) : BufferProcessor(options, parent, pool) {
  _current_input_buffer = NULL;
  initialize();
};

void EntropyMapper::close() {
  if(_current_input_buffer != NULL) _pool->release_buffer(_current_input_buffer);
}

int64_t EntropyMapper::cursor() {
  int64_t result = _entropy_cursor;
  if(_current_input_buffer != NULL) result += _current_input_buffer->source_offset();
  return result;
}

void EntropyMapper::initialize() {
  _entropy_cursor = 0;
  if(_parent != NULL) _entropy_cursor = _parent->cursor();
  if(_current_input_buffer != NULL) _pool->release_buffer(_current_input_buffer);
  _current_input_buffer = NULL;
  _current_output_buffer = NULL;
  _current_input_size = 0;
  _header_written = false;
  _entropy = 0;
  _bit_index = 0;
}

void EntropyMapper::fetch_next_buffer() {
  if(_current_input_buffer != NULL) _pool->release_buffer(_current_input_buffer);
  _current_input_buffer = _parent->next_buffer();
  _entropy_cursor = int64_t(0);
  if (_current_input_buffer != NULL) {
    _current_input_size = _current_input_buffer->size();
  }
}

EntropyMapper::~EntropyMapper() {
  this->close();
}

bool EntropyMapper::process_char(char input){

  return false;
}

void EntropyMapper::process_input_buffer() {
  if (_current_input_buffer == NULL) return;

  char* data = _current_input_buffer->buffer();
  while(_entropy_cursor < _current_input_size) {
    process_char(data[_entropy_cursor]);
    _entropy_cursor += 1;
  }
}

Buffer* EntropyMapper::write_header() {
  Buffer* output_buffer = _pool->get_buffer();
  char* buffer = output_buffer->buffer();
  uint32_t blocksize = _options->block_size() * _options->sample_blocks();

  // TODO this should be ceiled not floored
  uint64_t blocks = _options->length() / blocksize;

  // always use 2 bits to store the resolution, for now
  int numbits = 2;

  strcpy(buffer, "ENTF");
  memcpy(&buffer[4], &blocks, 8);
  memcpy(&buffer[12], &blocksize, 4);
  buffer[16] = numbits;

  for (int i = 17; i <= 32; i++) {
    buffer[i] = 0;
	}

  // the header length is 32 bytes
  output_buffer->set_size(32);

  return output_buffer;
}

Buffer* EntropyMapper::next_buffer() {
  if (_header_written == false) {
    _header_written = true;
    return write_header();
  }

  // get a new buffer
  if(_current_input_buffer == NULL) fetch_next_buffer();

  // process all the buffers
  while(_current_input_buffer != NULL) {
    process_input_buffer();

    // until it no longer seems to be fitting
    // TODO implement this
    if (true) return _current_output_buffer;

    fetch_next_buffer();
  }

  return _current_output_buffer;
}
