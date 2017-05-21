#include "catch.hpp"
#include "kr/JsonWritter.h"
#include "kr/logger.h"

TEST_CASE("JsonWritterTo")
{
    std::string t;
    kr::Json::WritterTo js(t);

    js.startObject();
    js.put("test", "test");
    js.endObject();

    REQUIRE(t == R"({"test":"test"})");
}

TEST_CASE("JsonWritter")
{
    kr::Json::Writter js;
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

    js.put("xxx");
    js.put("xxx2");
    REQUIRE(js.get() == R"("xxx","xxx2")");

    js.clear();
    REQUIRE(js.get() == "");
    std::string x = "xxx";
    js.put(x);
    REQUIRE(js.get() == R"("xxx")");
    js.put("x", x);
    REQUIRE(js.get() == R"("xxx","x":"xxx")");
}

TEST_CASE("JsonWritter complex object")
{
    kr::Json::Writter js;
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

    REQUIRE(
        js.get() ==
        R"({"data":{"array":[true,1,false,"ttt",1.500000,2.500000],"obj":true,"val":"val2"}})");
    js.clear();
    REQUIRE(js.get() == "");
}


TEST_CASE("JsonWritter escaping")
{
    REQUIRE(kr::Json::escape("\ntest\"") == R"(\ntest\")");
    REQUIRE(kr::Json::escape("\"\"\"\"") == R"(\"\"\"\")");
    REQUIRE(kr::Json::escape("\\") == R"(\\)");
    REQUIRE(kr::Json::escape("\\a") == R"(\\a)");
    REQUIRE(kr::Json::escape("\"\"\\\"\"") == R"(\"\"\\\"\")");
}
