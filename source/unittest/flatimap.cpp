#include "kr/flatimap.h"

#include "catch.hpp"
#include "kr/canaryobject.h"
#include <cstring>

#include <string>

using namespace std;
using namespace kr;

struct Data
{
    int id;
    std::string text {"xx"};

    Data(const std::string& _text)
        : text(_text) {};
};

TEST_CASE("flatimap basic int")
{
    flatimap<int, int> m;

    REQUIRE(m.size() == 0);
    REQUIRE(m.empty() == true);

    REQUIRE(m.has(4) == false);

    m.reserve(16);
    m.put(10, 4);
    REQUIRE(m.has(10) == true);
    REQUIRE(m[10] == 4);

    m.put(11, 5);
    REQUIRE(m.has(11) == true);
    REQUIRE(m[11] == 5);

    m.put(15, 9);
    REQUIRE(m.has(15) == true);
    REQUIRE(m[15] == 9);

    m.put(19, 3);
    REQUIRE(m.has(19) == true);
    REQUIRE(m[19] == 3);

    m.put(2, 90);
    REQUIRE(m.has(2) == true);
    REQUIRE(m[2] == 90);

    m.put(12, 94);
    REQUIRE(m.has(12) == true);
    REQUIRE(m[12] == 94);

    REQUIRE(m.has(4) == false);
    REQUIRE(m[2] == 90);
    REQUIRE(m[19] == 3);
    REQUIRE(m[15] == 9);
    REQUIRE(m[10] == 4);
    REQUIRE(m[11] == 5);
}
