#include "kr/inlineRing.h"
#include "catch.hpp"
#include "kr/optional.h"
#include <string>

#include <iostream>
#include <numeric>

TEST_CASE("inlineRing")
{
    kr::inlineRing8<int, 8> ring;

    ring.push_back(1);
    ring.push_back(2);
    ring.push_back(3);
    ring.push_back(4);
    REQUIRE(ring.size() == 4);
    ring.push_back(5);
    ring.push_back(6);
    ring.push_back(7);
    REQUIRE(not ring.full());
    ring.push_back(8);
    REQUIRE(ring.size() == 8);
    REQUIRE(ring.full());

    REQUIRE(ring.front() == 1);
    REQUIRE(ring.back() == 8);
    REQUIRE(ring[4] == 5);
    REQUIRE_THROWS_AS(ring.at(8), std::out_of_range);

    ring.pop_front();
    REQUIRE(ring[0] == 2);
    REQUIRE(ring.front() == 2);
    REQUIRE(ring.size() == 7);

    ring.emplace_front(10);
    REQUIRE(ring[0] == 10);
    REQUIRE(ring.front() == 10);
    REQUIRE(ring.back() == 8);
    REQUIRE(ring.size() == 8);

    ring.pop_front();
    ring.pop_front();
    ring.pop_front();
    ring.pop_front();
    ring.pop_front();
    ring.push_back(9);
    ring.push_back(10);
    ring.push_back(11);
    ring.push_back(12);
    REQUIRE(ring[0] == 6);
    REQUIRE(ring.front() == 6);
    REQUIRE(ring[6] == 12);
    REQUIRE(ring.back() == 12);
    REQUIRE(ring.size() == 7);

    int sum = 0;
    for (auto& i : ring)
        sum += i;
    REQUIRE(sum == (6 + 7 + 8 + 9 + 10 + 11 + 12));

    ring.erase(1);
    REQUIRE(std::accumulate(ring.begin(), ring.end(), 0) == 56);
    ring.erase(3);
    REQUIRE(std::accumulate(ring.begin(), ring.end(), 0) == 46);

    REQUIRE(ring.size() == 5);
    ring.insert(2, 13);
    REQUIRE(ring.size() == 6);

    REQUIRE(ring[0] == 6);
    REQUIRE(ring[1] == 8);
    REQUIRE(ring[2] == 13);

    REQUIRE(ring[3] == 9);
    REQUIRE(ring[4] == 11);
    REQUIRE(ring[5] == 12);
}
