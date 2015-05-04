#include "catch.hpp"
#include "options.h"
#include "testsuite.h"

TEST_CASE("Testing options") {
  SECTION("Parse empty options") {
    INFO("Test default values")
    const char* args[] = {"dit", "dat"};
    
    Options* options = parse_options(2, args);
    
    CHECK(options->offset() == 0);
    CHECK(options->block_size() == 32768);
    CHECK(options->length() == -1);
    CHECK(options->input_filename() == "dit");
    CHECK(options->output_filename() == "dat");
    CHECK(options->swap_bytes() == false);
    CHECK(options->quiet() == false);
    CHECK(options->append() == false);
    CHECK(options->search_bytes_length() == 0);
    CHECK(options->segment_offset() == 0);
    CHECK(options->segment_length() == 0);
    delete options;
  }
 
  SECTION("Set the offset") {
    const char* args[] = {"-o", "123", "dit", "dat"};
    Options* options = parse_options(4, args);
    CHECK(options->offset() == 123);
    delete options;
  }
  
  SECTION("Set the length") {
    const char* args[] = {"-l", "123", "dit", "dat"};
    Options* options = parse_options(4, args);
    CHECK(options->length() == 123);
    delete options;
  }
  
  
  SECTION("Set the end") {
    INFO("Gotcha: setting the end parameter will actually change the length parameter");
    const char* args[] = {"-o", "20", "-e", "123", "dit", "dat"};
    Options* options = parse_options(6, args);
    CHECK(options->check_arguments().compare("") == 0);
    CHECK(options->length() == 103);
    delete options;
  }
  
  SECTION("Set the swap-bytes") {
    const char* args[] = {"-s", "dit", "dat"};
    Options* options = parse_options(3, args);
    CHECK(options->swap_bytes() == true);
    delete options;
  }
  
  SECTION("Set the quiet") {
    const char* args[] = {"-q", "dit", "dat"};
    Options* options = parse_options(3, args);
    CHECK(options->quiet() == true);
    delete options;
  }
  
  SECTION("Set the append") {
    const char* args[] = {"-a", "dit", "dat"};
    Options* options = parse_options(3, args);
    CHECK(options->append() == true);
    delete options;
  }
  SECTION("Set the segment offset") {
    const char* args[] = {"-O", "4", "dit", "dat"};
    Options* options = parse_options(4, args);
    CHECK(options->segment_offset() == 4);
    delete options;
  }
  
  SECTION("Set the segment size") {
    const char* args[] = {"-L", "41", "dit", "dat"};
    Options* options = parse_options(4, args);
    CHECK(options->segment_length() == 41);
    delete options;
  }
  
  SECTION("Set the search chars") {
    const char* args[] = {"-c", "41", "dit", "dat"};
    Options* options = parse_options(4, args);
    CHECK(options->search_bytes_length() == 2);
    CHECK(memcmp(options->search_bytes(), "41", 2) == 0);
    delete options;
  }
  
  SECTION("Set the search bytes") {
    const char* args[] = {"-f", "0000414243", "dit", "dat"};
    Options* options = parse_options(4, args);
    CHECK(options->search_bytes_length() == 5);
    CHECK(memcmp(options->search_bytes(), "\x0\x0\x41\x42\x43", 3) == 0);
    delete options;
  }
  
  SECTION("Fail when end is before beginning"){
    const char* args[] = {"-o", "120", "-e", "100", "dit", "dat"};
    Options* options = parse_options(6, args);
    CHECK(options->check_arguments().compare("End needs to be bigger than offset") == 0);
    delete options;
  }

  SECTION("Fail when blocksize is smaller than  64"){
    const char* args[] = {"-b", "63", "dit", "dat"};
    Options* options = parse_options(4, args);
    CHECK(options->check_arguments().compare("The minimum block-size is 64 bytes") == 0);
    delete options;
  }
  
  SECTION("Fail when swapping bytes on uneven length"){
    const char* args[] = {"-s", "-l", "65", "dit", "dat"};
    Options* options = parse_options(5, args);
    CHECK(options->check_arguments().compare("You can only use swap-bytes with an even length") == 0);
    delete options;
  }

  SECTION("Fail when swapping bytes on uneven block size"){
    const char* args[] = {"-s", "-l", "64", "-b", "127", "dit", "dat"};
    Options* options = parse_options(7, args);
    CHECK(options->check_arguments().compare("You can only use swap-bytes with an even block size") == 0);
    delete options;
  }
  
  SECTION("Succeed when swapping bytes"){
    const char* args[] = {"-s", "-l", "64", "-b", "128", "dit", "dat"};
    Options* options = parse_options(7, args);
    std::string result = options->check_arguments();
    CHECK(result.compare("") == 0);
    delete options;
  }
}
