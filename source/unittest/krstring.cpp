#include "catch.hpp"
#include "kr/util/krstring.h"

TEST_CASE("krstring")
{
    REQUIRE(kr::itostr(1) == "1");
    REQUIRE(kr::itostr(5) == "5");
    REQUIRE(kr::itostr(165) == "165");
    REQUIRE(kr::itostr(234023) == "234023");
    REQUIRE(kr::itostr(0) == "0");
    REQUIRE(kr::itostr(-200) == "-200");
    REQUIRE(kr::itostr(-1) == "-1");
    REQUIRE(kr::itostr(-124) == "-124");
    REQUIRE(kr::itostr(266) == "266");
    REQUIRE(kr::itostr(255) == "255");
}
