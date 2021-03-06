#include "kr/paddedVector.h"
#include "catch.hpp"
#include "kr/canaryobject.h"

using namespace std;

struct Num
{
    Num() = default;
    Num(int i)
        : x(i) {};
    int x = 0;
};

TEST_CASE("paddedVector int")
{
    kr::paddedVector<Num, 8> v;

    REQUIRE(v.size() == 0);
    REQUIRE(v.element_size() == sizeof(Num) + 12);

    v.clear();

    v.emplace_back(1);

    REQUIRE(v.size() == 1);
    // REQUIRE(v[0].x == 1);

    v.push_back(Num());
    REQUIRE(v.size() == 2);
    // REQUIRE(v[1].x == 2);

    v.reserve(64);

    // v.push_back(1);
    v.clear();
}

TEST_CASE("paddedVector string")
{
    kr::paddedVector<std::string, 8> v;

    REQUIRE(v.size() == 0);
    REQUIRE(v.element_size() == sizeof(string) + 16);

    v.clear();

    v.emplace_back("test"s);
    // v.emplace_back("test2 a;sldkjasldkjas; ljdl;as jl;dj as;lj dl;kasj
    // l;dj;asj"s);

    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == "test");

    v.push_back("test2"s);
    REQUIRE(v.size() == 2);
    REQUIRE(v[1] == "test2");

    v.reserve(64);

    // v.push_back(1);
    v.clear();
}


class Canary2 : public kr::CanaryObject
{
  public:
    // string test = "x";

    // ~Canary2()
    // {
    //     // state = State::vdestruct;
    //     // states[_id] = State::vdestruct;
    // }
};

TEST_CASE("paddedVector Canary2")
{
    kr::paddedVector<kr::CanaryObject, 64> v;

    kr::CanaryObject::initStates();

    v.emplace_back(1, 1);
    REQUIRE(v.size() == 1);

    v.push_back(Canary2());

    // v.get<Canary2>(1).test = "test";
    // REQUIRE(v.get<Canary2>(1).test == "test");

    REQUIRE(kr::CanaryObject::states[0] == kr::CanaryObject::State::moveConst);
    REQUIRE(kr::CanaryObject::states[1] == kr::CanaryObject::State::construct2);

    REQUIRE(v.reserved() < 64);
    v.reserve(64);
    REQUIRE(v.reserved() == 64);

    // nothing will change, since we use memove
    REQUIRE(kr::CanaryObject::states[0] == kr::CanaryObject::State::moveConst);
    REQUIRE(kr::CanaryObject::states[1] == kr::CanaryObject::State::moveConst);


    // REQUIRE(v.get<Canary2>(1).test == "test");
    v.clear();
    REQUIRE(kr::CanaryObject::states[0] == kr::CanaryObject::State::destruct);
    REQUIRE(kr::CanaryObject::states[1] == kr::CanaryObject::State::destruct);
}
