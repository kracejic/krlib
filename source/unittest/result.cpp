#ifdef UNIT_TESTS
#include "kr/result.h"
#include "catch.hpp"
#include <string>

TEST_CASE("result")
{
    kr::result<int> empty;

    REQUIRE(empty.ok() == false);

    kr::result<int> val(4);
    REQUIRE(val.ok() == true);
    REQUIRE(*val == 4);
    REQUIRE(val.value() == 4);

    REQUIRE(empty.value_or(3) == 3);
    REQUIRE_THROWS_AS(empty.value(), std::range_error);
}


kr::result<std::string> funn(bool x)
{
    if (x)
        return {"ahoj"};
    else
        return {kr::FAILED, "ddd"};
}

// kr::result<void> funn3()
// {
//     
// }

TEST_CASE("result from function")
{
    auto empty = funn(false);
    REQUIRE(!empty);
    REQUIRE_THROWS_AS(empty.value(), std::range_error);
    REQUIRE(empty.value_or("33") == "33");

    auto val = funn(true);
    REQUIRE(val.ok() == true);
    REQUIRE(!!val == true);
    std::string x = *val;
    REQUIRE(x == "ahoj");
    REQUIRE(*val == "ahoj");
}

struct Vec
{
    int x, y;
    Vec(int _x, int _y)
        : x(_x)
        , y(_y) {};
    bool operator==(const Vec& other) const
    {
        return x == other.x && y == other.y;
    };
};

kr::result<Vec> funn2(bool x)
{
    if (x)
        return {1, 3};
    else
        return {};
}

TEST_CASE("")
{
    Vec x(1, 2);
    kr::result<Vec> empty;
    kr::result<Vec> val {1, 2};

    REQUIRE(!empty);
    REQUIRE(!!val);
    REQUIRE(*val == x);
    REQUIRE(empty.value_or({1, 2}) == x);
    REQUIRE(val->x == 1);
    REQUIRE(val->y == 2);

    if (!val)
        FAIL();
    if (empty)
        FAIL();

    auto val2 = funn2(false);
    REQUIRE(!val2);
    auto val3 = funn2(true);
    REQUIRE(!!val3);
    REQUIRE(!!funn2(true));
}

#include "kr/canaryobject.h"
TEST_CASE("result CanaryObject")
{
    kr::CanaryObject::initStates();
    {
        kr::result<kr::CanaryObject> val;
        REQUIRE(kr::CanaryObject::states[0] == kr::CanaryObject::State::undef);
    }
    // check it was not deconstructed
    REQUIRE(kr::CanaryObject::states[0] == kr::CanaryObject::State::undef);

    {
        kr::result<kr::CanaryObject> val1(2);
        REQUIRE(
            kr::CanaryObject::states[0] == kr::CanaryObject::State::construct2);
    }
    REQUIRE(kr::CanaryObject::states[0] == kr::CanaryObject::State::destruct);
}

#endif
