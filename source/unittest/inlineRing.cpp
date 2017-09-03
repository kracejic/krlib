#include "kr/inlineRing.h"
#include "catch.hpp"
#include "kr/optional.h"
#include <string>


TEST_CASE("inlineRing")
{
    kr::inlineRing<int,8> ring;
    
    ring.push_back(1);
    ring.push_back(2);
    ring.push_back(3);
    ring.push_back(4);
    REQUIRE(ring.size() == 4);
    ring.push_back(5);
    ring.push_back(6);
    ring.push_back(7);
    ring.push_back(8);
    REQUIRE(ring.size() == 8);
    
    REQUIRE(ring.front() == 1);
    REQUIRE(ring.back() == 8);
}
