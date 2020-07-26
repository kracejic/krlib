#include "kr/inlineVec.h"
#include "catch.hpp"
#include "kr/optional.h"
#include <string>

using namespace std;

kr::inlineVec<int, 10> fun_int()
{
    kr::inlineVec<int, 10> t;
    t.push_back(3);
    return t;
}
TEST_CASE("inlineVec int")
{
    kr::inlineVec<int, 10> t;
    REQUIRE(not t.full());

    t.push_back(5);
    t.push_back(3);

    REQUIRE(not t.full());
    REQUIRE(t[0] == 5);
    REQUIRE(t.at(1) == 3);
    REQUIRE_THROWS_AS(t.at(2), std::out_of_range);

    t.push_back(1);
    int sum = 0;
    for (auto& i : t)
        sum += i;
    REQUIRE(sum == 9);

    kr::inlineVec<int, 2> t2;
    REQUIRE(not t2.full());
    t2.push_back(2);
    REQUIRE(not t2.full());
    t2.push_back(2);
    REQUIRE(t2.full());

    t = fun_int();
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
        , x(_x) {};
};
kr::inlineVec<Test, 13> fun_test()
{
    kr::inlineVec<Test, 13> t;
    return t;
}
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

    t = fun_test();
}

#include "kr/canaryobject.h"
kr::CanaryObject::State kr::CanaryObject::states[256];

kr::inlineVec<kr::CanaryObject, 13> fun()
{
    kr::inlineVec<kr::CanaryObject, 13> t;
    return t;
}

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

    kr::CanaryObject c {0, 1};
    REQUIRE(c.state == kr::CanaryObject::State::construct2);
    REQUIRE(kr::CanaryObject::states[1] == kr::CanaryObject::State::construct2);
    t.push_back(std::move(c));
    REQUIRE(c.state == kr::CanaryObject::State::movedFrom);
    REQUIRE(kr::CanaryObject::states[1] == kr::CanaryObject::State::moveConst);
    REQUIRE(t.back().state == kr::CanaryObject::State::moveConst);

    t = fun();
}


TEST_CASE("inlineVec erase")
{
    kr::CanaryObject::initStates();
    kr::inlineVec<kr::CanaryObject, 13> t;

    t.emplace_back(1, 1);
    t.emplace_back(2, 2);
    t.emplace_back(3, 3);
    t.emplace_back(4, 4);

    t.erase(1);
    REQUIRE(t.size() == 3);

    REQUIRE(t[0].state == kr::CanaryObject::State::construct2);
    REQUIRE(t[0].val == 1);
    REQUIRE(t[1].state == kr::CanaryObject::State::moveAssign);
    REQUIRE(t[1].val == 3);
    REQUIRE(t[2].state == kr::CanaryObject::State::moveAssign);
    REQUIRE(t[2].val == 4);
    REQUIRE(t[3].state == kr::CanaryObject::State::movedFrom);
    REQUIRE(kr::CanaryObject::states[1] == kr::CanaryObject::State::construct2);
    REQUIRE(kr::CanaryObject::states[2] == kr::CanaryObject::State::destruct);
    REQUIRE(kr::CanaryObject::states[3] == kr::CanaryObject::State::moveAssign);
    REQUIRE(kr::CanaryObject::states[4] == kr::CanaryObject::State::moveAssign);
}


TEST_CASE("inlineVec insert")
{
    kr::CanaryObject::initStates();
    kr::inlineVec<kr::CanaryObject, 13> t;

    t.emplace_back(1, 1);
    t.emplace_back(2, 2);
    t.emplace_back(3, 3);
    t.emplace_back(4, 4);

    REQUIRE(t.size() == 4);

    REQUIRE(t[0].val == 1);
    t.insert(1, {5, 5});
    REQUIRE(t.size() == 5);
    REQUIRE(t[0].val == 1);
    REQUIRE(t[1].val == 5);
    REQUIRE(t[2].val == 2);
    REQUIRE(t[3].val == 3);
    REQUIRE(t[4].val == 4);
    REQUIRE(t[1].state == kr::CanaryObject::State::moveAssign);
    REQUIRE(kr::CanaryObject::states[5] == kr::CanaryObject::State::moveAssign);

    REQUIRE(kr::CanaryObject::states[1] == kr::CanaryObject::State::construct2);
    REQUIRE(kr::CanaryObject::states[2] == kr::CanaryObject::State::moveAssign);
    REQUIRE(kr::CanaryObject::states[3] == kr::CanaryObject::State::moveAssign);
    REQUIRE(kr::CanaryObject::states[4] == kr::CanaryObject::State::moveConst);
}
