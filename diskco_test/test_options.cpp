#include "catch.hpp"
#include "options.h"
#include "testsuite.h"

TEST_CASE("Testing options") {
  SECTION("Parse empty options") {
    INFO("Test default values")
    Options* options = parse_options(2, std::move((const char*[]){ "dit", "dat"}));
    
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
    Options* options = parse_options(4, std::move((const char*[]){"-o", "123", "dit", "dat"}));
    CHECK(options->offset() == 123);
    delete options;
  }
  
  SECTION("Set the length") {
    Options* options = parse_options(4, std::move((const char*[]){"-l", "123", "dit", "dat"}));
    CHECK(options->length() == 123);
    delete options;
  }
  
  
  SECTION("Set the end") {
    INFO("Gotcha: setting the end parameter will actually change the length parameter");
    Options* options = parse_options(6, std::move((const char*[]){"-o", "20", "-e", "123", "dit", "dat"}));
    CHECK(options->check_arguments().compare("") == 0);
    CHECK(options->length() == 103);
    delete options;
  }
  
  SECTION("Set the swap-bytes") {
    Options* options = parse_options(3, std::move((const char*[]){"-s", "dit", "dat"}));
    CHECK(options->swap_bytes() == true);
    delete options;
  }
  
  SECTION("Set the quiet") {
    Options* options = parse_options(3, std::move((const char*[]){"-q", "dit", "dat"}));
    CHECK(options->quiet() == true);
    delete options;
  }
  
  SECTION("Set the append") {
    Options* options = parse_options(3, std::move((const char*[]){"-a", "dit", "dat"}));
    CHECK(options->append() == true);
    delete options;
  }
  SECTION("Set the segment offset") {
    Options* options = parse_options(4, std::move((const char*[]){"-O", "4", "dit", "dat"}));
    CHECK(options->segment_offset() == 4);
    delete options;
  }
  
  SECTION("Set the segment size") {
    Options* options = parse_options(4, std::move((const char*[]){"-L", "41", "dit", "dat"}));
    CHECK(options->segment_length() == 41);
    delete options;
  }
  
  SECTION("Set the search chars") {
    Options* options = parse_options(4, std::move((const char*[]){"-c", "41", "dit", "dat"}));
    CHECK(options->search_bytes_length() == 2);
    CHECK(strcmp(options->search_bytes(), "41") == 0);
    delete options;
  }
  
  SECTION("Set the search bytes") {
    Options* options = parse_options(4, std::move((const char*[]){"-f", "0000414243", "dit", "dat"}));
    CHECK(options->search_bytes_length() == 5);
    CHECK(memcmp(options->search_bytes(), "\x0\x0\x41\x42\x43", 3) == 0);
    delete options;
  }
  
  SECTION("Fail when end is before beginning"){
    Options* options = parse_options(6, std::move((const char*[]){"-o", "120", "-e", "100", "dit", "dat"}));
    CHECK(options->check_arguments().compare("End needs to be bigger than offset") == 0);
    delete options;
  }

  SECTION("Fail when blocksize is smaller than  64"){
    Options* options = parse_options(4, std::move((const char*[]){"-b", "63", "dit", "dat"}));
    CHECK(options->check_arguments().compare("The minimum block-size is 64 bytes") == 0);
    delete options;
  }
  
  SECTION("Fail when swapping bytes on uneven length"){
    Options* options = parse_options(5, std::move((const char*[]){"-s", "-l", "65", "dit", "dat"}));
    CHECK(options->check_arguments().compare("You can only use swap-bytes with an even length") == 0);
    delete options;
  }

  SECTION("Fail when swapping bytes on uneven block size"){
    Options* options = parse_options(7, std::move((const char*[]){"-s", "-l", "64", "-b", "127", "dit", "dat"}));
    CHECK(options->check_arguments().compare("You can only use swap-bytes with an even block size") == 0);
    delete options;
  }
  
  SECTION("Succeed when swapping bytes"){
    Options* options = parse_options(7, std::move((const char*[]){"-s", "-l", "64", "-b", "128", "dit", "dat"}));
    std::string result = options->check_arguments();
    CHECK(result.compare("") == 0);
    delete options;
  }
}
