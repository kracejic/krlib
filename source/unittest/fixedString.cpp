#include "kr/fixedString.h"

#include "catch.hpp"

#include <string>

using namespace std;

TEST_CASE("fixedString")
{
    kr::fixedString<8> s;

    std::string a = "";
    REQUIRE(a.length() == 0);
    REQUIRE(a.size() == 0);
    a = "1";
    REQUIRE(a.length() == 1);
    REQUIRE(a.size() == 1);

    REQUIRE(s.length() == 0);
    s = "1";
    REQUIRE(s.length() == 1);
    s = "ahoj";
    REQUIRE(s.length() == 4);
    s = a;
    REQUIRE(s.length() == 1);


    kr::fixedString<8> x{"1"};
    REQUIRE(x == s);
    x = "1ssssdd";
    REQUIRE(x != s);

}
