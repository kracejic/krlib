#include "catch.hpp"
#include "kr/JsonReader.h"

#include <iostream>


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


TEST_CASE("JsonReader simple array")
{
    std::cout << "--------------- \n";
    Reader r{R"([85, "xx", ["aa" ,1, true, null,false ], -13, 3.6])"};

    REQUIRE(r.isArray());
    REQUIRE(r.size() == 5);
    REQUIRE(r[1].size() == 0);
    REQUIRE(r[2].size() == 5);

    for (auto& e : r)
    {
        REQUIRE(not e.isObject());
    }

    REQUIRE(r[0] == 85);
    REQUIRE(r[1] == "xx");
    REQUIRE(r[2][0] == "aa");
    REQUIRE(r[2][1] == 1);
    REQUIRE(r[2][2] == true);
    REQUIRE(r[2][3] == nullptr);
    REQUIRE(r[2][4] == false);
    REQUIRE(r[3] == -13);
    REQUIRE(r[4] == 3.6);
}


