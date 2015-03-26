//
//  file_reader.cpp
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#include "file_reader.h"
FileReader::FileReader(Options* options) : BufferProcessor(options) {
  _file = fopen(options->input_filename().c_str(), "rb");
  if(!_file) throw std::runtime_error("Could not open input file");
  fseek(_file, 0, SEEK_END);
  _file_size = ftell(_file);
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
  fseek(_file, _options->offset(), SEEK_SET);
  _cursor = _options->offset();
  // if length is not set, read the entire file!
  if (_options->length() == -1) _options->set_length(_file_size - _options->offset());
  _bytes_left = _options->length();
  if(_progress) delete _progress;
  _progress = new Progress(_options->offset(), _options->offset() + _options->length());
}

FileReader::~FileReader() {
  this->close();
  delete _file;
}

int FileReader::process(Buffer *buffer) {
  if(feof(_file) || _bytes_left == 0) return -1;
  int64_t read_bytes = buffer->capacity();
  if (_bytes_left < read_bytes) read_bytes = _bytes_left;
  size_t bytes_read = fread(buffer->buffer(), 1, read_bytes, _file);
  buffer->set_size(bytes_read);
  _cursor += bytes_read;
  if (_bytes_left > 0) _bytes_left -= bytes_read;
  if (!_options->quiet()) _progress->update(_cursor);
  return 0;
}