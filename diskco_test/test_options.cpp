#include "catch.hpp"
#include "options.h"

Options *parse_options(int argc, const char* args[]){
  char* argv[argc+1];
  argv[0] = (char*)("test");
  for(int i=0; i<argc;i++) argv[i+1] = (char*) args[i];
  return new Options(argc + 1, argv);
}

TEST_CASE("Testing options") {
  SECTION("Parse empty options") {
    INFO("Test default values")
    Options* options = parse_options(2, (const char*[]){ "dit", "dat"});
    
    CHECK(options->offset() == 0);
    CHECK(options->block_size() == 32768);
    CHECK(options->length() == -1);
    CHECK(options->input_filename() == "dit");
    CHECK(options->output_filename() == "dat");
    CHECK(options->swap_bytes() == false);
    CHECK(options->quiet() == false);
    CHECK(options->append() == false);
    delete options;
  }
 
  SECTION("Set the offset") {
    Options* options = parse_options(4, (const char*[]){"-o", "123", "dit", "dat"});
    CHECK(options->offset() == 123);
    delete options;
  }
  
  SECTION("Set the length") {
    Options* options = parse_options(4, (const char*[]){"-l", "123", "dit", "dat"});
    CHECK(options->length() == 123);
    delete options;
  }
  
  
  SECTION("Set the end") {
    INFO("Gotcha: setting the end parameter will actually change the length parameter");
    Options* options = parse_options(6, (const char*[]){"-o", "20", "-e", "123", "dit", "dat"});
    CHECK(options->check_arguments().compare("") == 0);
    CHECK(options->length() == 103);
    delete options;
  }
  
  SECTION("Set the swap-bytes") {
    Options* options = parse_options(3, (const char*[]){"-s", "dit", "dat"});
    CHECK(options->swap_bytes() == true);
    delete options;
  }
  
  SECTION("Set the quiet") {
    Options* options = parse_options(3, (const char*[]){"-q", "dit", "dat"});
    CHECK(options->quiet() == true);
    delete options;
  }
  
  SECTION("Set the append") {
    Options* options = parse_options(3, (const char*[]){"-a", "dit", "dat"});
    CHECK(options->append() == true);
    delete options;
  }
  
  
  SECTION("Fail when end is before beginning"){
    Options* options = parse_options(6, (const char*[]){"-o", "120", "-e", "100", "dit", "dat"});
    CHECK(options->check_arguments().compare("End needs to be bigger than offset") == 0);
    delete options;
  }

  SECTION("Fail when blocksize is smaller than  64"){
    Options* options = parse_options(4, (const char*[]){"-b", "63", "dit", "dat"});
    CHECK(options->check_arguments().compare("The minimum block-size is 64 bytes") == 0);
    delete options;
  }
  
  SECTION("Fail when swapping bytes on uneven length"){
    Options* options = parse_options(5, (const char*[]){"-s", "-l", "65", "dit", "dat"});
    CHECK(options->check_arguments().compare("You can only use swap-bytes with an even length") == 0);
    delete options;
  }

  SECTION("Fail when swapping bytes on uneven block size"){
    Options* options = parse_options(7, (const char*[]){"-s", "-l", "64", "-b", "127", "dit", "dat"});
    CHECK(options->check_arguments().compare("You can only use swap-bytes with an even block size") == 0);
    delete options;
  }
  
  SECTION("Succeed when swapping bytes"){
    Options* options = parse_options(7, (const char*[]){"-s", "-l", "64", "-b", "128", "dit", "dat"});
    std::string result = options->check_arguments();
    CHECK(result.compare("") == 0);
    delete options;
  }
}