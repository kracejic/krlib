#ifdef UNIT_TESTS
#include "kr/optional.h"
#include "catch.hpp"
#include <string>

TEST_CASE("optional")
{
    kr::optional<int> empty;

    REQUIRE(empty.has_value() == false);

    kr::optional<int> val(4);
    REQUIRE(val.has_value() == true);
    REQUIRE(*val == 4);
    REQUIRE(val.value() == 4);

    REQUIRE(empty.value_or(3) == 3);
    REQUIRE_THROWS_AS(empty.value(), std::range_error);
}


kr::optional<std::string> fun(bool x)
{
    if (x)
        return {"ahoj"};
    else
        return {};
}

TEST_CASE("optional from function")
{
    auto empty = fun(false);
    REQUIRE(!empty);
    REQUIRE_THROWS_AS(empty.value(), std::range_error);
    REQUIRE(empty.value_or("33") == "33");

    auto val = fun(true);
    REQUIRE(val.has_value() == true);
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

kr::optional<Vec> fun2(bool x)
{
    if (x)
        return {1, 3};
    else
        return {};
}

TEST_CASE("")
{
    Vec x(1, 2);
    kr::optional<Vec> empty;
    kr::optional<Vec> val {1, 2};

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

    auto val2 = fun2(false);
    REQUIRE(!val2);
    auto val3 = fun2(true);
    REQUIRE(!!val3);
    REQUIRE(!!fun2(true));
}

#include "kr/canaryobject.h"
TEST_CASE("CanaryObject")
{
    kr::CanaryObject::initStates();
    {
        kr::optional<kr::CanaryObject> val;
        REQUIRE(kr::CanaryObject::states[0] == kr::CanaryObject::State::undef);
    }
    // check it was not deconstructed
    REQUIRE(kr::CanaryObject::states[0] == kr::CanaryObject::State::undef);

    {
        kr::optional<kr::CanaryObject> val1(2);
        REQUIRE(
            kr::CanaryObject::states[0] == kr::CanaryObject::State::construct2);
    }
    REQUIRE(kr::CanaryObject::states[0] == kr::CanaryObject::State::destruct);
}

#endif
