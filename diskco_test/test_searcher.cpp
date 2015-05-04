#include "catch.hpp"
#include <utility>
#include "testsuite.h"
#include "options.h"
#include "searcher.h"
#include "buffer_pool.h"



TEST_CASE("Testing searcher") {
  BufferPool* pool = new BufferPool(5,256);
  mock_input_file("/tmp/test_diskco.dat",256);
  const char* args[] = {"-q", "-f", "0001", "-O", "0", "-L", "3", "/tmp/test_diskco.dat", "dat"};
  Options* options = parse_options(9, args);

  
  SECTION("Does a search and finds a result at pos 0") {
    FileReader* parent = new FileReader(options, NULL, pool);
    Searcher* searcher = new Searcher(options, parent, pool);
    Buffer* buffer = searcher->next_buffer();
    CHECK(buffer->size() == 3);
    CHECK(buffer->buffer()[0] == 0);
    CHECK(buffer->buffer()[1] == 1);
    CHECK(buffer->buffer()[2] == 2);
    CHECK(buffer->source_offset() ==0);
    pool->release_buffer(buffer);
    delete parent;
  }
  
  SECTION("Does a search, finds nothing, returns NULL") {
    FileReader* parent = new FileReader(options, NULL, pool);
    options->set_search_bytes("\x04\x07");
    Searcher* searcher = new Searcher(options, parent, pool);
    Buffer* buffer = searcher->next_buffer();
    CHECK(buffer == NULL);
    delete parent;
  }

  SECTION("Does a search and finds a result at pos 254, returns less bytes than asked") {
    FileReader* parent = new FileReader(options, NULL, pool);
    options->set_search_bytes("\xFE\xFF");
    Searcher* searcher = new Searcher(options, parent, pool);
    Buffer* buffer = searcher->next_buffer();
    CHECK(buffer->size() == 2);
    CHECK((unsigned char)buffer->buffer()[0] == 254);
    CHECK((unsigned char)buffer->buffer()[1] == 255);
    pool->release_buffer(buffer);
    delete parent;
  }
  
  SECTION("Does not find an incomplete pattern") {
    FileReader* parent = new FileReader(options, NULL, pool);
    options->set_search_bytes("\xFE\xFF\x03");
    Searcher* searcher = new Searcher(options, parent, pool);
    Buffer* buffer = searcher->next_buffer();
    CHECK(buffer == NULL);
    delete parent;
  }
  
  
  SECTION("Finds a result on a block boundary") {
    mock_input_file("/tmp/test_diskco.dat",512);
    FileReader* parent = new FileReader(options, NULL, pool);
    options->set_search_bytes(std::string("\xFE\xFF\x00\x01", 4));
    
    Searcher* searcher = new Searcher(options, parent, pool);
    Buffer* buffer = searcher->next_buffer();
    CHECK(buffer != NULL);
    if (buffer != NULL) {
      CHECK(buffer->size() == 3);
      CHECK((unsigned char)buffer->buffer()[0] == 254);
      CHECK((unsigned char)buffer->buffer()[1] == 255);
      CHECK((unsigned char)buffer->buffer()[2] == 0);
      pool->release_buffer(buffer);
    }
    delete parent;
  }
  
  
  delete options;
  delete pool;
}
