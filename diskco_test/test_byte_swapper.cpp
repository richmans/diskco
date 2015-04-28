#include "catch.hpp"
#include <utility>
#include "testsuite.h"
#include "options.h"
#include "byte_swapper.h"
#include "buffer_pool.h"

void prepare_buffer(Buffer* buffer) {
  for(int i=0;i < buffer->capacity(); i++) {
    buffer->buffer()[i] = i;
  }
  buffer->set_size(buffer->capacity());
}

TEST_CASE("Testing byteswapper") {
  BufferPool* pool = new BufferPool(256,512);
  Options* options = parse_options(2, std::move((const char*[]){ "dit", "dat"}));
  MockBufferProcessor* parent = new MockBufferProcessor(options, pool, 0);
  ByteSwapper* swapper = new ByteSwapper(options, parent, pool);
  Buffer* buffer = pool->get_buffer();

  SECTION("Swaps 2 bytes") {
    prepare_buffer(buffer);
    parent->set_next_buffer(buffer);
    CHECK(buffer->buffer()[0] == 0);
    CHECK(buffer->buffer()[1] == 1);
    
    swapper->next_buffer();
    
    CHECK(buffer->buffer()[0] == 1);
    CHECK(buffer->buffer()[1] == 0);
  }
  
  SECTION("Swaps 3 bytes") { 
    prepare_buffer(buffer);
    parent->set_next_buffer(buffer);
    CHECK(buffer->buffer()[0] == 0);
    CHECK(buffer->buffer()[1] == 1);
    CHECK(buffer->buffer()[2] == 2);
    buffer->set_size(3);
    swapper->next_buffer();
    
    CHECK(buffer->buffer()[0] == 1);
    CHECK(buffer->buffer()[1] == 0);
    CHECK(buffer->buffer()[2] == 2);
  }
  
  delete buffer;
  delete swapper;
  delete options;
  

}
