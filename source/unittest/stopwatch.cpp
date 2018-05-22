#include "kr/stopwatch.h"
#include "catch.hpp"
#include "kr/multistopwatch.h"

#include "iostream"
#include <thread>

using namespace std::literals::chrono_literals;

void foo(kr::Stopwatch& t)
{
    KR_STOPWATCH_LAP(t);
}

TEST_CASE("Stopwatch")
{
    kr::Stopwatch t;
    t.start();
    foo(t);
    std::this_thread::sleep_for(20ms);
    KR_STOPWATCH_LAP(t);
    REQUIRE(t.ms() > 18);
    REQUIRE(t.s() < 0.03);
    KR_STOPWATCH_LAP(t);
}

TEST_CASE("MultiStopwatch")
{
    kr::MultiStopwatch t;
    t.start("first");
    std::this_thread::sleep_for(20ms);
    t.lap("second");
    std::this_thread::sleep_for(10ms);
    t.lap("null");
    t.lap("null2");
    t.lap("1ms");
    std::this_thread::sleep_for(1ms);
    t.lap("null4");
    t.lap("null5");
    auto ret = t.finalize();
    REQUIRE(ret[0].duration_ms > 18);
    REQUIRE(ret[0].duration_ms < 22);
    REQUIRE(ret[1].duration_ms > 8);
    REQUIRE(ret[1].duration_ms < 12);

    for (auto& i : ret)
    {
        std::cout << i.str() << std::endl;
    }
}
