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
    CHECK(pretty_bytes(1) == "1  B");
    CHECK(pretty_bytes(1024) == "1 KB");
    CHECK(pretty_bytes(1029) == "1 KB");
    CHECK(pretty_bytes(1024*1024) == "1 MB");
    CHECK(pretty_bytes(1024*1024*1024) == "1 GB");
    CHECK(pretty_bytes(int64_t(1024)*1024*1024*1024) == "1 TB");
  }

  SECTION("hextobytes") {
    INFO("Converting hexadecimal to bytes")
    CHECK(std::memcmp(hextobytes("00"),"\x0", 1) == 0);
    CHECK(std::memcmp(hextobytes("20"), " ",1) == 0);
    CHECK(std::memcmp(hextobytes("7454455354"), "tTEST", 5) == 0);
  }
}
