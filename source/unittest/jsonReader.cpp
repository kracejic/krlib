#include "catch.hpp"
#include "kr/JsonReader.h"

using namespace kr::Json;
TEST_CASE("JsonReader simple jsons")
{
    kr::Json::Reader r{R"("text")"};
    REQUIRE(r.isString());

    kr::Json::Reader r2{R"(562)"};
    REQUIRE(not r2.isString());
    REQUIRE(r2.isNumber());

    REQUIRE(Reader("+1").isNumber());
    REQUIRE(Reader("-1").isNumber());
    REQUIRE(Reader("+1.32").isNumber());
    REQUIRE(Reader("-1.43").isNumber());
}






