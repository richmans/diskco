//
//  file_reader.cpp
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#include "file_reader.h"
FileReader::FileReader(Options* options, BufferProcessor* parent, BufferPool* pool) : BufferProcessor(options, parent, pool) {
  _file = fopen(options->input_filename().c_str(), "rb");
  if(!_file) throw std::runtime_error("Could not open input file from FileReader");
  _progress = NULL;
  initialize();
};

void FileReader::close() {
  if(_file != NULL) {
    fclose(_file);
    _file = NULL;
    if((!_options->quiet()) && _progress) {
      _progress->finish();
    }
  }
}

void FileReader::initialize() {
  initialize(_options->offset(), _options->length());
}

void FileReader::initialize(int64_t offset, int64_t length){
  fseek(_file, offset, SEEK_SET);
  _cursor = offset;
  _bytes_left = length;
  if(_progress) delete _progress;
  _progress = new Progress(offset, offset + length);
}

FileReader::~FileReader() {
  this->close();
  delete _file;
}

Buffer* FileReader::next_buffer() {
  if(feof(_file) || _bytes_left == 0) return NULL;
  Buffer* buffer= _pool->get_buffer();
  int64_t read_bytes = buffer->capacity();

  if (_bytes_left < read_bytes) read_bytes = _bytes_left;
  size_t bytes_read = fread(buffer->buffer(), 1, read_bytes, _file);
  buffer->set_size(bytes_read);
  buffer->set_source_offset(_cursor);
  _cursor += bytes_read;
  if (_bytes_left > 0) _bytes_left -= bytes_read;
  if (!_options->quiet()) _progress->update(_cursor);
  return buffer;
}

int64_t FileReader::cursor() {
  return _cursor;
}
