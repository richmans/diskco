#include "catch.hpp"
#include <utility>
#include "testsuite.h"
#include "options.h"
#include "byte_swapper.h"
#include "buffer_pool.h"

using namespace std;

TEST_CASE("Testing byteswapper") {
  BufferPool* pool = new BufferPool(256,512);
  const char* args[] = {"dit", "dat"};
  Options* options = parse_options(2, args);
  MockBufferProcessor* parent = new MockBufferProcessor(options, pool, 0);
  ByteSwapper* swapper = new ByteSwapper(options, parent, pool);
  Buffer* buffer = pool->get_buffer();

  SECTION("Swaps 2 bytes") {
    prepare_buffer(buffer);
    parent->set_next_buffer(buffer);
    CHECK(buffer->buffer()[0] == 0);
    CHECK(buffer->buffer()[1] == 1);
    CHECK(buffer->buffer()[2] == 2);
    CHECK(buffer->buffer()[3] == 3);
    
    swapper->next_buffer();
    
    CHECK(buffer->buffer()[0] == 1);
    CHECK(buffer->buffer()[1] == 0);
    CHECK(buffer->buffer()[2] == 3);
    CHECK(buffer->buffer()[3] == 2);
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
