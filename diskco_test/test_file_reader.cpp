#include "catch.hpp"
#include <utility>
#include "testsuite.h"
#include "options.h"
#include "file_reader.h"
#include "buffer_pool.h"



TEST_CASE("Testing filereader") {
  BufferPool* pool = new BufferPool(256,512);
  const char* args[] = {"-q", "/tmp/test_diskco.dat", "dat"};
  Options* options = parse_options(3, args);
  
  SECTION("Reads one buffer") {
    mock_input_file("/tmp/test_diskco.dat", 512);
    FileReader* reader = new FileReader(options, NULL, pool);
    
    Buffer* buffer = reader->next_buffer();
    CHECK(buffer->buffer()[0] == 0);
    CHECK(buffer->buffer()[1] == 1);
    CHECK(buffer->buffer()[2] == 2);
    CHECK((unsigned char)(buffer->buffer()[200]) == 200);
    CHECK(buffer->source_offset() == 0);
    pool->release_buffer(buffer);
    CHECK(reader->next_buffer() == NULL);
    delete reader;
  }
  
  SECTION("Reads a partial buffer") {
    mock_input_file("/tmp/test_diskco.dat", 513);
    FileReader* reader = new FileReader(options, NULL, pool);
    Buffer* buffer = reader->next_buffer();
    CHECK(buffer->buffer()[0] == 0);
    CHECK(buffer->buffer()[1] == 1);
    CHECK(buffer->buffer()[2] == 2);
    CHECK(buffer->source_offset() == 0);
    CHECK((unsigned char)(buffer->buffer()[200]) == 200);
    pool->release_buffer(buffer);
    buffer = reader->next_buffer();
    CHECK(buffer->size() == 1);
    CHECK(buffer->buffer()[0] == 0);
    pool->release_buffer(buffer);
    CHECK(reader->next_buffer() == NULL);
    delete reader;
  }
  
  SECTION("Reads at an offset") {
    mock_input_file("/tmp/test_diskco.dat", 513);
    const char* args[] = {"-o", "2", "-l", "3", "-q", "/tmp/test_diskco.dat", "dat"};
    Options* options2 = parse_options(7, args);
    FileReader* reader = new FileReader(options2, NULL, pool);
    Buffer* buffer = reader->next_buffer();
    CHECK(buffer->size() == 3);
    CHECK(buffer->buffer()[0] == 2);
    CHECK(buffer->buffer()[1] == 3);
    CHECK(buffer->buffer()[2] == 4);
    CHECK(buffer->source_offset() == 2);
    pool->release_buffer(buffer);
    delete reader;
    delete options2;
  }
  delete options;
  delete pool;
}
