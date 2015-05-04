#include "catch.hpp"
#include <utility>
#include "testsuite.h"
#include "options.h"
#include "file_writer.h"
#include "buffer_pool.h"



TEST_CASE("Testing filewriter") {
  BufferPool* pool = new BufferPool(256,512);
  const char* args[] = {"-q", "/tmp/test_diskco.dat", "/tmp/test_diskco_out.dat"};
  Options* options = parse_options(3, args);
  MockBufferProcessor* parent = new MockBufferProcessor(options, pool, 2);
  Buffer* buffer = pool->get_buffer();
  
  
  SECTION("Writes one buffer") {
    remove("/tmp/test_diskco_out.dat");
    FileWriter* writer = new FileWriter(options, parent, pool);
    
    Buffer* buffer = writer->next_buffer();
    CHECK(buffer->buffer()[0] == 0);
    CHECK(buffer->buffer()[1] == 1);
    CHECK(buffer->buffer()[2] == 2);
    writer->close();
    FILE* file = fopen("/tmp/test_diskco_out.dat", "rb");
    CHECK(file != NULL);
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    CHECK(file_size == 512);
    delete writer;
  }

  delete parent;
  delete options;
  delete pool;
}
