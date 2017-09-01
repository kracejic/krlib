#pragma once
#include <chrono>
#include <string>

namespace kr
{

class SpeedTimer
{
  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> mStart;
    std::chrono::time_point<std::chrono::high_resolution_clock> mEnd;

  public:
    void start()
    {
        mStart = std::chrono::high_resolution_clock::now();
    };
    void stop()
    {
        mEnd = std::chrono::high_resolution_clock::now();
    };
    std::string str()
    {
        return std::to_string(
                   std::chrono::duration_cast<
                       std::chrono::duration<double, std::ratio<1, 1000>>>(
                       mEnd - mStart)
                       .count()) +
               "ms";
    };
    double getSec()
    {
        return std::chrono::duration_cast<std::chrono::duration<double>>(
                   mEnd - mStart)
            .count();
    };
    double getMSec()
    {
        return std::chrono::duration_cast<
                   std::chrono::duration<double, std::ratio<1, 1000>>>(
                   mEnd - mStart)
            .count();
    };
    std::string stop_print_restart()
    {
        stop();
        auto ret = str();
        start();
        return ret;
    }
};

} /* kr */
