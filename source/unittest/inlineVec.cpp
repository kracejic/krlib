#include "kr/inlineVec.h"
#include "catch.hpp"
#include "kr/optional.h"
#include <string>

using namespace std;

TEST_CASE("inlineVec int")
{
    kr::inlineVec<int, 10> t;

    t.push_back(5);
    t.push_back(3);

    REQUIRE(t[0] == 5);
    REQUIRE(t.at(1) == 3);
    REQUIRE_THROWS_AS(t.at(2), std::out_of_range);

    t.push_back(1);
    int sum = 0;
    for (auto& i : t)
        sum += i;
    REQUIRE(sum == 9);
}

TEST_CASE("inlineVec string")
{
    kr::inlineVec<string, 10> t;

    t.push_back("hello");
    t.push_back(" world");

    REQUIRE(t[0] == "hello");
    REQUIRE(t.at(1) == " world");
    REQUIRE_THROWS_AS(t.at(2), std::out_of_range);

    t.push_back("!");
    string sum = "";
    for (auto& i : t)
        sum += i;
    REQUIRE(sum == "hello world!");
}

struct Test
{
    int a;
    unsigned char x;
    Test() = default;
    Test(int _a, unsigned char _x)
        : a(_a)
        , x(_x){};
};
TEST_CASE("inlineVec struct")
{
    kr::inlineVec<Test, 13> t;

    REQUIRE(t.size() == 0);
    t.emplace_back();
    t.emplace_back(1000, 5);
    REQUIRE(t.size() == 2);
    t.emplace_back();
    REQUIRE(t.size() == 3);
    t.pop_back();
    REQUIRE(t.size() == 2);
    t.clear();
    REQUIRE(t.size() == 0);
}

#include "kr/canaryobject.h"
kr::CanaryObject::State kr::CanaryObject::states[256];

TEST_CASE("inlineVec canary")
{
    kr::CanaryObject::initStates();
    kr::inlineVec<kr::CanaryObject, 13> t;
    
    REQUIRE(kr::CanaryObject::states[0] == kr::CanaryObject::State::undef);
    t.emplace_back();
    REQUIRE(kr::CanaryObject::states[0] == kr::CanaryObject::State::construct1);
    t.pop_back();
    REQUIRE(kr::CanaryObject::states[0] == kr::CanaryObject::State::destruct);
    t.emplace_back(2);
    REQUIRE(kr::CanaryObject::states[0] == kr::CanaryObject::State::construct2);
    t.pop_back();
    REQUIRE(kr::CanaryObject::states[0] == kr::CanaryObject::State::destruct);

    kr::CanaryObject c{0,1};
    REQUIRE(c.state == kr::CanaryObject::State::construct2);
    REQUIRE(kr::CanaryObject::states[1] == kr::CanaryObject::State::construct2);
    t.push_back(std::move(c));
    REQUIRE(c.state == kr::CanaryObject::State::movedFrom);
    REQUIRE(kr::CanaryObject::states[1] == kr::CanaryObject::State::moveConst);
    REQUIRE(t.back().state == kr::CanaryObject::State::moveConst);
}
