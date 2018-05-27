#include "catch.hpp"
#include "kr/canaryobject.h"
#include "kr/indexvector.h"
#include <cstring>

#include <string>

using namespace std;
using namespace kr;

struct data
{
    int id;
    std::string text{"xx"};

    data(const std::string& _text)
        : text(_text){};
};

TEST_CASE("indexvector basic int")
{
    indexvector<int, data> v;
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);
    v.reserve(32);
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 32);
    REQUIRE(v.has(0) == false);

    auto key1 = v.push_back({"ahoj"}).id;
    REQUIRE(v.size() == 1);
    REQUIRE(key1 == 0);
    REQUIRE(v[key1].text == "ahoj");

    auto key2 = v.push_back({"test"}).id;
    REQUIRE(key2 == 1);
    REQUIRE(v.size() == 2);
    REQUIRE(v[key2].text == "test");

    v.erase(key2);
    REQUIRE(v.size() == 1);
    REQUIRE(v[key1].text == "ahoj");

    auto key3 = v.push_back({"ddd"}).id;
    REQUIRE(key3 == 1);
    REQUIRE(v.size() == 2);
    REQUIRE(v[key3].text == "ddd");

    v.erase(key1);
    REQUIRE(v.size() == 1);


    REQUIRE_THROWS_AS(v.at(0), std::out_of_range);
    REQUIRE(v.at(key3).text == "ddd");
    REQUIRE_THROWS_AS(v.at(2), std::out_of_range);
    REQUIRE_THROWS_AS(v.at(3), std::out_of_range);

    REQUIRE(v.has(0) == false);
    REQUIRE(v.has(key3) == true);
    REQUIRE(v.has(2) == false);
    REQUIRE(v.has(3) == false);
    REQUIRE(v.has(4) == false);
    REQUIRE(v.has(-1) == false);
    REQUIRE(v.has(27) == false);
    REQUIRE(v.has(28) == false);
    REQUIRE(v.has(29) == false);
    REQUIRE(v.has(30) == false);
    REQUIRE(v.has(31) == false);
    REQUIRE(v.has(32) == false);
    REQUIRE(v.has(33) == false);
    REQUIRE(v.has(456) == false);

    // int* x;
    // x = new int[5]();
    // memset((void*)x, 0xff, 5*sizeof(int));
    // REQUIRE(x[0] == -1);
}


class CanaryID : public CanaryObject
{
  public:
    CanaryID(int canary_id)
        : CanaryObject(0, canary_id){};
    int id;
    string data{""};
};

TEST_CASE("indexvector with Canary")
{
    using State = CanaryObject::State;
    CanaryObject::initStates();
    {
        indexvector<int, CanaryID> v;
        v.reserve(4);


        REQUIRE(v.size() == 0);
        auto obj1 = CanaryID(1);
        auto key1 = v.push_back(std::move(obj1)).id;
        REQUIRE(v[key1].kr::CanaryObject::state == State::moveConst);
        v[key1].data = "xxx";
        REQUIRE(obj1.state == State::movedFrom);

        auto obj2 = CanaryID(2);
        auto key2 = v.push_back(obj2).id;
        REQUIRE(v[key2].kr::CanaryObject::state == State::copyConst);
        REQUIRE(obj2.state == State::construct2);
        v[key2].data = "abc";

        auto key3 = v.emplace_back(3).id;
        REQUIRE(v[key3].kr::CanaryObject::state == State::construct2);
        v[key3].data = "some longer text that needs allocation";

        int count = 0;
        for (auto& i : v)
        {
            REQUIRE(i.state != State::undef);
            count++;
        }
        REQUIRE(count == 3);

        REQUIRE(v[key1].data == "xxx");
        REQUIRE(v[key2].data == "abc");
        REQUIRE(v[key3].data == "some longer text that needs allocation");

        v.reserve(16);
        REQUIRE(v[key1].data == "xxx");
        REQUIRE(v[key2].data == "abc");
        REQUIRE(v[key3].data == "some longer text that needs allocation");
        REQUIRE(v.size() == 3);

        REQUIRE(kr::CanaryObject::states[0] == State::undef);
        REQUIRE(kr::CanaryObject::states[1] == State::moveConst);
        REQUIRE(kr::CanaryObject::states[2] == State::moveConst);
        REQUIRE(kr::CanaryObject::states[3] == State::moveConst);
        REQUIRE(kr::CanaryObject::states[4] == State::undef);
    }

    REQUIRE(kr::CanaryObject::states[0] == State::undef);
    REQUIRE(kr::CanaryObject::states[1] == State::destruct);
    REQUIRE(kr::CanaryObject::states[2] == State::destruct);
    REQUIRE(kr::CanaryObject::states[3] == State::destruct);
    REQUIRE(kr::CanaryObject::states[4] == State::undef);
}

TEST_CASE("indexvector stressing")
{
    indexvector<int, CanaryID> v;


    REQUIRE(true);
}

// TODO performance tests
