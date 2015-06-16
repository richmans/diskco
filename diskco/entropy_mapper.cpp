//
//  searcher.cpp
//  Diskco
//
//  Created by Richard Bronkhorst on 12/05/15.
//  Copyright (c) 2015 Martin van Wingerden All rights reserved.
//

#include "entropy_mapper.h"
#include <ostream>
#include <math.h>

EntropyMapper::EntropyMapper(Options* options, BufferProcessor* parent, BufferPool* pool) : BufferProcessor(options, parent, pool) {
  _current_input_buffer = NULL;
  _histo_map = new int[256];
  initialize();
};

void EntropyMapper::close() {
  if(_current_input_buffer != NULL) _pool->release_buffer(_current_input_buffer);
  if(_current_output_buffer != NULL) _pool->release_buffer(_current_output_buffer);
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
  _current_input_size = 0;
  _current_output_buffer = NULL;
  _output_cursor_step = 0;
  _header_written = false;
  _entropy = 0;
  _zeroes = 0;

  // initialize the histo_map
  for (int i = 0; i < 256; i++) _histo_map[i] = 0;
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
  delete _histo_map;
}

void EntropyMapper::process_entropy()
{
  int location = _output_cursor_step / 4;
  int bit_index = _output_cursor_step % 4;

  // we are the first one to write in this memory location
  if (bit_index == 0){
    char* buffer = _current_output_buffer->buffer();
    buffer[location] = '\0';
  }

  int entropy = calculate_entropy();
  _current_output_buffer->buffer()[location] |= (entropy << (bit_index * 2));
  _current_output_buffer->set_size(location + 1);

  _zeroes = 0;
  for (int i = 0; i < 256; i++) _histo_map[i] = 0;

  _output_cursor_step++;
}

int EntropyMapper::calculate_entropy(){
  int blocksize = _options->processing_block_size();

  if (_options->entropy_algorithm() == nulls){
    if (_zeroes == blocksize) {
  		return 0;
  	} else if (_zeroes > blocksize/2) {
  		return 1;
  	} else if (_zeroes > blocksize/10) {
  		return 2;
  	} else {
  		return 3;
  	}
  } else {
    _entropy = 0.0;

    for (int i = 0; i < 256; i++)
    {
      if (_histo_map[i] == 0) continue;

      double p = (double)_histo_map[i] / (double)blocksize;
      if (p > 0.0){
        _entropy -= p * log2(p);
      }
    }

    if (_entropy == 0.0) {
  		return 0;
  	} else if (_entropy < 2.0) {
  		return 1;
  	} else if (_entropy < 7.0) {
  		return 2;
  	} else {
  		return 3;
  	}
  }
}

void EntropyMapper::process_input_buffer() {
  if (_current_input_buffer == NULL) return;
  bool count_zeroes = _options->entropy_algorithm() == nulls;
  uint64_t processing_block_size = _options->processing_block_size();

  int block_cursor = 0;
  char* data = _current_input_buffer->buffer();

  while (_entropy_cursor < _current_input_size) {

    if (count_zeroes){
      if (data[_entropy_cursor] == 0) _zeroes++;
    } else {
      _histo_map[(unsigned char)data[_entropy_cursor]] += 1;
    }

    _entropy_cursor++;
    block_cursor++;

    if (block_cursor == processing_block_size){
      process_entropy();
      block_cursor = 0;
    }
  }

  // if the cursor is not on a boundary
  if (_entropy_cursor % processing_block_size != 0){
    process_entropy();
  }
}

Buffer* EntropyMapper::write_header() {
  Buffer* output_buffer = _pool->get_buffer();
  char* buffer = output_buffer->buffer();
  uint32_t blocksize = _options->processing_block_size() * _options->sample_blocks();
  uint64_t blocks = _options->length() / blocksize;
  if (_options->length() % blocksize > 0) blocks++;

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

bool EntropyMapper::will_an_extra_input_buffer_fit()
{
  int64_t remaining = _current_output_buffer->capacity() - _current_output_buffer->size();
  int64_t needed = _options->block_size() / _options->processing_block_size()  / 4;

  return (remaining - needed) >= 0;
}

Buffer* EntropyMapper::next_buffer() {
  if (_header_written == false) {
    _header_written = true;
    return write_header();
  }

  // get a new buffer, we have none
  if(_current_input_buffer == NULL) fetch_next_buffer();

  // we did not get data back from the fetch buffer
  if(_current_input_buffer == NULL) return NULL;

  _current_output_buffer = _pool->get_buffer();
  _output_cursor_step = 0;

  // process all the buffers
  while(_current_input_buffer != NULL) {

    process_input_buffer();

    // until it no longer seems to be fitting
    if (!will_an_extra_input_buffer_fit()){
      return _current_output_buffer;
    }

    fetch_next_buffer();
  }

  return _current_output_buffer;
}
