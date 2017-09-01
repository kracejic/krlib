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

TEST_CASE("JsonReader simple object")
{
    Reader r{R"({"test":"first", "num":56, "array":[5,6,"test"]})"};
    REQUIRE(r.size() == 3);
    REQUIRE(r[0].size() == 2);
    REQUIRE(r[0].str() == R"("test":"first")");
    REQUIRE(r[0].key() == "test");
    REQUIRE(r[0].value() == "first");

    REQUIRE(r["test"] == "first");
    REQUIRE(r["num"] == 56);
    REQUIRE(r["array"][0] == 5);
    REQUIRE(r["array"][1] == 6);
    REQUIRE(r["array"][2] == "test");

    for (auto& e : r)
    {
        REQUIRE(e.key().size() >= 3);
        REQUIRE(not e.value().isObject());
    }

    std::string def = "xx";

    REQUIRE(r.get_or("test", "xx") == "first");
    REQUIRE(r.get_or("tes", "xx") == "xx");
    REQUIRE(r.get_or("tests", "xx") == "xx");
    REQUIRE(r.get_or("tests", def) == "xx");
    REQUIRE(r.get_or("num", 100) == 56);
    REQUIRE(r.get_or("num2", 100) == 100);
    REQUIRE(r.get_or("nu", 100) == 100);
    REQUIRE(r.get_or("nu", 5.6) == 5.6);

    // std::cout << "r.text.size() = " << r.text.size() << "\n";
    // size_t s = (int8_t*)&(*(r.tree.end())) - (int8_t*)&(*(r.tree.begin()));
    // std::cout << "s = " << s << "\n";
    // std::cout << "r.tree.size() = " << r.tree.size() << "\n";
}


