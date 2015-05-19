#include "catch.hpp"
#include "options.h"
#include "testsuite.h"
#include <iostream>
#include <string>

TEST_CASE("Testing options") {
  mock_input_file("/tmp/test_diskco.dat", 512);
  char *filename = new char[21];
  std::strcpy(filename, "/tmp/test_diskco.dat");

  SECTION("Parse empty options") {
    INFO("Test default values")
    const char* args[] = {filename, "dat"};

    Options* options = parse_options(2, args);

    CHECK(options->offset() == 0);
    CHECK(options->block_size() == 32768);

    // note that the length is overwritten with the actual file length
    CHECK(options->length() == 512);
    CHECK(options->input_filename() == filename);
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
    const char* args[] = {"-o", "123", filename, "dat"};
    Options* options = parse_options(4, args);
    CHECK(options->offset() == 123);
    delete options;
  }

  SECTION("Set the length") {
    const char* args[] = {"-l", "123", filename, "dat"};
    Options* options = parse_options(4, args);
    CHECK(options->length() == 123);
    delete options;
  }


  SECTION("Set the end") {
    INFO("Gotcha: setting the end parameter will actually change the length parameter");
    const char* args[] = {"-o", "20", "-e", "123", filename, "dat"};
    Options* options = parse_options(6, args);
    printf("%s", options->check_arguments().c_str());
    CHECK(options->check_arguments().compare("") == 0);
    CHECK(options->length() == 103);
    delete options;
  }

  SECTION("Set the swap-bytes") {
    const char* args[] = {"-s", filename, "dat"};
    Options* options = parse_options(3, args);
    CHECK(options->swap_bytes() == true);
    delete options;
  }

  SECTION("Set the quiet") {
    const char* args[] = {"-q", filename, "dat"};
    Options* options = parse_options(3, args);
    CHECK(options->quiet() == true);
    delete options;
  }

  SECTION("Set the append") {
    const char* args[] = {"-a", filename, "dat"};
    Options* options = parse_options(3, args);
    CHECK(options->append() == true);
    delete options;
  }
  SECTION("Set the segment offset") {
    const char* args[] = {"-O", "4", filename, "dat"};
    Options* options = parse_options(4, args);
    CHECK(options->segment_offset() == 4);
    delete options;
  }

  SECTION("Set the segment size") {
    const char* args[] = {"-L", "41", filename, "dat"};
    Options* options = parse_options(4, args);
    CHECK(options->segment_length() == 41);
    delete options;
  }

  SECTION("Set the search chars") {
    const char* args[] = {"-c", "41", filename, "dat"};
    Options* options = parse_options(4, args);
    CHECK(options->search_bytes_length() == 2);
    CHECK(memcmp(options->search_bytes(), "41", 2) == 0);
    delete options;
  }

  SECTION("Set the search bytes") {
    const char* args[] = {"-f", "0000414243", filename, "dat"};
    Options* options = parse_options(4, args);
    CHECK(options->search_bytes_length() == 5);
    CHECK(memcmp(options->search_bytes(), "\x0\x0\x41\x42\x43", 3) == 0);
    delete options;
  }

  SECTION("Fail when end is before beginning"){
    const char* args[] = {"-o", "120", "-e", "100", filename, "dat"};
    Options* options = parse_options(6, args);
    CHECK(options->check_arguments().compare("End needs to be bigger than offset") == 0);
    delete options;
  }

  SECTION("Fail when blocksize is smaller than  64"){
    const char* args[] = {"-b", "63", filename, "dat"};
    Options* options = parse_options(4, args);
    CHECK(options->check_arguments().compare("The minimum block-size is 64 bytes") == 0);
    delete options;
  }

  SECTION("Fail when swapping bytes on uneven length"){
    const char* args[] = {"-s", "-l", "65", filename, "dat"};
    Options* options = parse_options(5, args);
    CHECK(options->check_arguments().compare("You can only use swap-bytes with an even length") == 0);
    delete options;
  }

  SECTION("Fail when swapping bytes on uneven block size"){
    const char* args[] = {"-s", "-l", "64", "-b", "127", filename, "dat"};
    Options* options = parse_options(7, args);
    CHECK(options->check_arguments().compare("You can only use swap-bytes with an even block size") == 0);
    delete options;
  }

  SECTION("Succeed when swapping bytes"){
    const char* args[] = {"-s", "-l", "64", "-b", "128", filename, "dat"};
    Options* options = parse_options(7, args);
    std::string result = options->check_arguments();
    CHECK(result.compare("") == 0);
    delete options;
  }

  SECTION("Set the entropy parameter"){
    const char* args[] = {"-E", filename, "dat"};
    Options* options = parse_options(3, args);
    std::string result = options->check_arguments();
    CHECK(options->entropy_algorithm() == nulls);
    CHECK(options->processing_block_size() == 512);
    CHECK(options->sample_blocks() == 1);
    CHECK(result.compare("") == 0);
    delete options;
  }

  SECTION("Set the entropy parameter and some related options"){
    const char* args[] = {"-E", "-B", "64", "-S", "1024", filename, "dat"};
    Options* options = parse_options(7, args);
    std::string result = options->check_arguments();
    CHECK(options->entropy_algorithm() == nulls);
    CHECK(options->processing_block_size() == 64);

    // the reading blocksize is automaticly adjusted to be the processing block size
    CHECK(options->block_size() == 64);
    CHECK(options->sample_blocks() == 1024);
    CHECK(result.compare("") == 0);
    delete options;
  }

  SECTION("Set a specific entropy algorithm"){
    const char* args[] = {"-Elog", filename, "dat"};
    Options* options = parse_options(3, args);
    std::string result = options->check_arguments();
    CHECK(options->entropy_algorithm() == logarithmic);
    CHECK(result.compare("") == 0);
    delete options;
  }
}
