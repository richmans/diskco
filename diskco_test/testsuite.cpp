
#include "testsuite.h"
void prepare_buffer(Buffer* buffer) {
  for(int i=0;i < buffer->capacity(); i++) {
    buffer->buffer()[i] = (unsigned char)i;
  }
  buffer->set_size(buffer->capacity());
}

Options *parse_options(int argc, const char* args[]){
  char* argv[argc+1];
  argv[0] = (char*)("test");
  for(int i=0; i<argc;i++) argv[i+1] = (char*) args[i];
  return new Options(argc + 1, argv);
}

void mock_input_file(std::string file_name, int length){
  std::string flags;
  flags = "w+b";
  FILE* file = fopen(file_name.c_str(), flags.c_str());
  
  if(file == NULL) {
    throw std::runtime_error("Could not write to output file");
  }
  
  Buffer* buffer = new Buffer(length);
  prepare_buffer(buffer);
  fwrite(buffer->buffer(), 1, buffer->size(), file);
  fclose(file);
  delete buffer;
}