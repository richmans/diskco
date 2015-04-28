#include "catch.hpp"
#include <utility>
#include "testsuite.h"
#include "options.h"
#include "file_reader.h"
#include "buffer_pool.h"



TEST_CASE("Testing filereader") {
  BufferPool* pool = new BufferPool(256,512);
  Options* options = parse_options(3, std::move((const char*[]){"-q", "/tmp/test_diskco.dat", "dat"}));
  
  SECTION("Reads one buffer") {
    mock_input_file("/tmp/test_diskco.dat", 512);
    FileReader* reader = new FileReader(options, NULL, pool);
    
    Buffer* buffer = reader->next_buffer();
    CHECK(buffer->buffer()[0] == 0);
    CHECK(buffer->buffer()[1] == 1);
    CHECK(buffer->buffer()[2] == 2);
    CHECK((unsigned char)(buffer->buffer()[200]) == 200);

    pool->release_buffer(buffer);
    CHECK(reader->next_buffer() == NULL);
  }
  
  SECTION("Reads a partial buffer") {
    mock_input_file("/tmp/test_diskco.dat", 513);
    FileReader* reader = new FileReader(options, NULL, pool);
    Buffer* buffer = reader->next_buffer();
    CHECK(buffer->buffer()[0] == 0);
    CHECK(buffer->buffer()[1] == 1);
    CHECK(buffer->buffer()[2] == 2);
    CHECK((unsigned char)(buffer->buffer()[200]) == 200);
    pool->release_buffer(buffer);
    buffer = reader->next_buffer();
    CHECK(buffer->size() == 1);
    CHECK(buffer->buffer()[0] == 0);
    pool->release_buffer(buffer);
    CHECK(reader->next_buffer() == NULL);
  }
}
