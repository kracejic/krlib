#include "kr/flatimap.h"

#include "kr/multistopwatch.h"
#include "kr/stopwatch.h"

#include "catch.hpp"
#include "kr/canaryobject.h"
#include <cstring>
#include <flat_map>

#include "iostream"
#include <string>

using namespace std;
using namespace kr;

struct Data
{
    int id;
    std::string text {"xx"};

    Data(const std::string& _text)
        : text(_text) { };
};

TEST_CASE("flatimap basic int")
{
    flatimap<int, int> m;

    REQUIRE(m.size() == 0);
    REQUIRE(m.empty() == true);

    REQUIRE(m.contains(4) == false);

    m.reserve(16);
    m.put(10, 4);
    REQUIRE(m.contains(10) == true);
    REQUIRE(m[10] == 4);

    m.put(11, 5);
    REQUIRE(m.contains(11) == true);
    REQUIRE(m[11] == 5);

    m.put(15, 9);
    REQUIRE(m.contains(15) == true);
    REQUIRE(m[15] == 9);

    m.put(19, 3);
    REQUIRE(m.contains(19) == true);
    REQUIRE(m[19] == 3);

    m.put(2, 90);
    REQUIRE(m.contains(2) == true);
    REQUIRE(m[2] == 90);

    m.put(12, 94);
    REQUIRE(m.contains(12) == true);
    REQUIRE(m[12] == 94);

    REQUIRE(m.contains(4) == false);
    REQUIRE(m[2] == 90);
    REQUIRE(m[19] == 3);
    REQUIRE(m[15] == 9);
    REQUIRE(m[10] == 4);
    REQUIRE(m[11] == 5);
}

struct TestObj
{
    int id;
    int x;
    int ar[20];
    int y;
};

#include <map>
#include <unordered_map>

template <class T>
int filling(T& m)
{
    int ret = 0;
    for (int i = 0; i < 10000; ++i)
    {
        m.clear();
        for (int ii = 0; ii < 150; ++ii)
        {
            int index = rand() % 100000;
            m.insert_or_assign(index, TestObj());
            // m[index].x = rand() % 100000;
        }
    }
    return ret;
}

#define MAXID 5000

template <class T>
int randomizeData(T& m)
{
    int ret = 0;
    m.clear();
    for (int i = 0; i < 30; ++i)
    {
        int index = rand() % MAXID;
        m.insert_or_assign(index, TestObj());
        index = rand() % MAXID;
        m.insert_or_assign(index, TestObj());

        while (true)
        {
            index = rand() % MAXID;
            if (m.contains(index))
            {
                m.erase(index);
                break;
            }
        }
    }
    return ret;
}


template <class T>
int access(T& m)
{
    int ret = 0;
    for (int i = 0; i < 100000; ++i)
    {
        int index = rand() % MAXID;
        if (m.contains(index) == true)
            m[index].x = rand() % 500;
    }
    return ret;
}

template <class T>
int process(T& m)
{
    int ret = 0;
    for (int i = 0; i < 1; ++i)
    {
        for (auto x : m)
            ret += x.second.x;
    }
    return ret;
}
template <class T>
int process2(T& m)
{
    int ret = 0;
    for (int i = 0; i < 1; ++i)
    {
        for (auto& x : m)
            ret += x.x;
    }
    return ret;
}

TEST_CASE("Perftest")
{
    srand(0);

    kr::MultiStopwatch t;
    map<int, TestObj> t_map;
    unordered_map<int, TestObj> t_map2;
    flatimap<int, TestObj> t_map3;
    std::flat_map<int, TestObj> t_map4;

    t.clearAndLap("fill - map");
    filling(t_map);
    t.lap("fill - unordered_map");
    filling(t_map2);
    t.lap("fill - flatimap");
    filling(t_map3);
    t.lap("fill - std::flat_map");
    filling(t_map4);
    auto ret = t.finalize();
    for (auto& i : ret)
        std::cout << i.str() << std::endl;
    std::cout << endl;

    t.clearAndLap("randomize - map");
    randomizeData(t_map);
    t.lap("randomize - unordered_map");
    randomizeData(t_map2);
    t.lap("randomize - flatimap");
    randomizeData(t_map3);
    t.lap("randomize - std::flat_map");
    randomizeData(t_map4);
    ret = t.finalize();
    for (auto& i : ret)
        std::cout << i.str() << std::endl;
    std::cout << endl;

    t.clearAndLap("access - map");
    access(t_map);
    t.lap("access - unordered_map");
    access(t_map2);
    t.lap("access - flatimap");
    access(t_map3);
    t.lap("access - std::flat_map");
    access(t_map4);
    ret = t.finalize();
    for (auto& i : ret)
        std::cout << i.str() << std::endl;
    std::cout << endl;

    t.clearAndLap("process - map");
    process(t_map);
    t.lap("process - unordered_map");
    process(t_map2);
    t.lap("process - flatimap");
    process2(t_map3);
    t.lap("process - std::flat_map");
    process(t_map4);
    ret = t.finalize();
    for (auto& i : ret)
        std::cout << i.str() << std::endl;
    std::cout << endl;

    REQUIRE(true);
}
