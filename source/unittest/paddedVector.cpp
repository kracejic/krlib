#include "kr/paddedVector.h"
#include "catch.hpp"
#include "kr/canaryobject.h"

using namespace std;

TEST_CASE("paddedVector")
{
    kr::paddedVector<std::string, 8> v;
    
    REQUIRE(v.size() == 0);
    REQUIRE(v.element_size() == sizeof(string)+16);

    v.clear();

    v.emplace_back("test");

    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == "test");

    // v.push_back("test2");
    // REQUIRE(v.size() == 2);
    // REQUIRE(v[1] == "test2");
}

