//-----------------------------------------------------------------------------
#ifdef UNIT_TESTS
#include "kr/explicit.h"
#include "catch.hpp"
#include <string>


using Test1 = kr::NamedType<float, struct Test1Tag>;

TEST_CASE("explicit")
{
    Test1 x = Test1(1.5);
    Test1 y = Test1(1.5);
    Test1 z(0.5);

    REQUIRE(x == y);
    REQUIRE(z != y);

    REQUIRE(to_string(z) == "0.500000");
}

using Test2 = kr::NamedType<std::string, struct Test2Tag>;

TEST_CASE("explicit2")
{
    Test2 x = Test2("ahoj");
    Test2 y = Test2("ahoj");
    Test2 z("test");

    REQUIRE(x == y);
    REQUIRE(z != y);

    REQUIRE(to_string(z) == "test");
}

using Distance = kr::NamedNumberType<int, struct DistanceTag, 3>;
using Distance2 = kr::NamedNumberType<int, struct DistanceTag2, 3>;
TEST_CASE("explicit numbered")
{
    Distance c;
    REQUIRE(c == Distance {3});

    Distance x {5};
    REQUIRE(x == Distance {5});

    REQUIRE(x + Distance {3} == Distance {8});
    x += Distance {2};
    REQUIRE(x == Distance {7});
    REQUIRE(to_string(x) == "7");

    REQUIRE(x - Distance {5} == Distance {2});
    x -= Distance {2};
    REQUIRE(x == Distance {5});

    REQUIRE(x * Distance {2} == Distance {10});
    x *= Distance {2};
    REQUIRE(x == Distance {10});

    REQUIRE(x / Distance {2} == Distance {5});
    x /= Distance {2};
    REQUIRE(x == Distance {5});
    REQUIRE(x != Distance {7});


    Distance a {1}, b {2}, d {2};

    REQUIRE(a < b);
    REQUIRE(a <= b);
    REQUIRE(b <= d);
    REQUIRE(b >= d);
    REQUIRE(b >= a);
    REQUIRE(b >= a);

    REQUIRE(not(b < a));
    REQUIRE(not(b <= a));
    REQUIRE(not(a >= b));
    REQUIRE(not(a >= b));
}
#endif
