#include "catch.hpp"
#include "utils.h"
TEST_CASE("Testing utils") {
    SECTION("Bytesize") {
        INFO("Converting arguments to numbers of bytes")
        CHECK(bytesize("1") == 1);
        CHECK(bytesize("1K") == 1024);
        CHECK(bytesize("1k") == 1024);
        CHECK(bytesize("1m") == 1024*1024);
        CHECK(bytesize("1M") == 1024*1024);
        CHECK(bytesize("1g") == 1024*1024*1024);
        CHECK(bytesize("1G") == 1024*1024*1024);
        CHECK(bytesize("1t") == int64_t(1024)*1024*1024*1024);
        CHECK(bytesize("1T") == int64_t(1024)*1024*1024*1024);
    }
  
  SECTION("pretty_byte") {
    INFO("Converting number of bytes to pretty text")
    CHECK(pretty_bytes(1) == "1  b");
    CHECK(pretty_bytes(1024) == "1 Kb");
    CHECK(pretty_bytes(1029) == "1 Kb");
    CHECK(pretty_bytes(1024*1024) == "1 Mb");
    CHECK(pretty_bytes(1024*1024*1024) == "1 Gb");
    CHECK(pretty_bytes(int64_t(1024)*1024*1024*1024) == "1 Tb");
  }
}