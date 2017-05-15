#include "kr/JsonWritter.h"
#include "kr/logger.h"
#include "catch.hpp"

TEST_CASE("JsonWritter")
{
    // REQUIRE(kr::itostr(255) == "255");
    kr::JsonWritter js;
    js.startArray();
    js.endArray();
    REQUIRE(js.get() == "[]");
    js.startObject();
    js.endObject();
    REQUIRE(js.get() == "[],{}");
    js.startObject("test1");
    js.endObject();
    REQUIRE(js.get() == "[],{},\"test1\":{}");
    js.startArray("test2");
    js.endArray();
    REQUIRE(js.get() == "[],{},\"test1\":{},\"test2\":[]");

    std::string a = js.moveText();
    REQUIRE(js.get() == "");
}

TEST_CASE("JsonWritter complex object")
{
    kr::JsonWritter js;
    js.startObject();
    js.startObject("data");
    js.startArray("array");
    js.put(true);
    js.put(1);
    js.put(false);
    js.put("ttt");
    js.put(1.5f);
    js.put(2.5);
    js.endArray();
    js.put("obj", true);
    js.put("val", "val2");
    js.endObject();
    js.endObject();

    REQUIRE(js.get() == R"({"data":{"array":[true,1,false,"ttt",1.500000,2.500000],"obj":true,"val":"val2"}})");
    js.clear();
    REQUIRE(js.get() == "");
}

