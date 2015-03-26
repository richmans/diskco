#include "catch.hpp"
#include "testsuite.h"
#include "options.h"
#include "byte_swapper.h"

void prepare_buffer(Buffer* buffer) {
  for(int i=0;i < buffer->capacity(); i++) {
    buffer->buffer()[i] = i;
  }
  buffer->set_size(buffer->capacity());
}

TEST_CASE("Testing byteswapper") {
 
  Options* options = parse_options(2, (const char*[]){ "dit", "dat"});
  ByteSwapper* swapper = new ByteSwapper(options);
  Buffer* buffer = new Buffer(512);

  SECTION("Swaps 2 bytes") {
    prepare_buffer(buffer);
    CHECK(buffer->buffer()[0] == 0);
    CHECK(buffer->buffer()[1] == 1);
    
    swapper->process(buffer);
    
    CHECK(buffer->buffer()[0] == 1);
    CHECK(buffer->buffer()[1] == 0);
  }
  
  SECTION("Swaps 3 bytes") {
    prepare_buffer(buffer);
    CHECK(buffer->buffer()[0] == 0);
    CHECK(buffer->buffer()[1] == 1);
    CHECK(buffer->buffer()[2] == 2);
    buffer->set_size(3);
    swapper->process(buffer);
    
    CHECK(buffer->buffer()[0] == 1);
    CHECK(buffer->buffer()[1] == 0);
    CHECK(buffer->buffer()[2] == 2);
  }
  
  delete buffer;
  delete swapper;
  delete options;
  

}