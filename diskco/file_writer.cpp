//
//  file_writer.cpp
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#include "file_writer.h"



FileWriter::FileWriter(Options* options) : BufferProcessor(options) {
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

int FileWriter::process(Buffer *buffer) {
  fwrite(buffer->buffer(), 1, buffer->size(), _file);
  return 0;
}