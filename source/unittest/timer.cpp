#include "kr/timer.h"
#include "catch.hpp"

#include "iostream"
#include <thread>

using namespace std::literals::chrono_literals;

void foo(kr::SpeedTimer& t)
{
    KR_TIMER_LAP(t);
}

TEST_CASE("timer")
{
    kr::SpeedTimer t;
    t.start();
    REQUIRE(true);
    foo(t);
    std::this_thread::sleep_for(20ms);
    KR_TIMER_LAP(t);
    REQUIRE(t.ms() > 18);
    REQUIRE(t.s() < 0.03);
    KR_TIMER_LAP(t);
}
