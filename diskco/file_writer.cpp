//
//  file_writer.cpp
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#include "file_writer.h"



FileWriter::FileWriter(Options* options, BufferProcessor* parent, BufferPool* pool) : BufferProcessor(options, parent, pool) {
  std::string flags;
  if (_options->append()) {
    flags = "ab";
  }else{
    flags = "w+b";
  }
  _file = fopen(options->output_filename().c_str(), flags.c_str());
  if(_file == NULL) {
    throw std::runtime_error("Could not open output file");
  }
};

FileWriter::~FileWriter() {
  close();
}

void FileWriter::close() {
  if(_file != NULL) {
    fclose(_file);
    _file = NULL;
  }
  
}

Buffer* FileWriter::next_buffer() {
  Buffer* buffer = _parent->next_buffer();
  if(buffer == NULL) return NULL;
  printf("Writing %lld bytes\n", buffer->size());
  fwrite(buffer->buffer(), 1, buffer->size(), _file);
  _pool->release_buffer(buffer);
  return buffer;
}