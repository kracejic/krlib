#pragma once
#include <chrono>
#include <memory>
#include <string>

namespace kr
{

#define KR_STOPWATCH_LAP(timer)                                                \
    std::cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " - "   \
              << timer.lap_str() << std::endl


class Stopwatch
{
  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> mStart;
    std::chrono::duration<double, std::ratio<1, 1000>> mDuration;

  public:
    /// helper global singleton instance
    static Stopwatch& global()
    {
        thread_local static std::unique_ptr<Stopwatch> instance;
        if (not instance)
            instance.reset(new Stopwatch());
        return *instance;
    };

    /// marks start of the timer
    void start()
    {
        mStart = std::chrono::high_resolution_clock::now();
    };

    /// calculates duration from start and restarts the counter
    void lap()
    {
        auto end = std::chrono::high_resolution_clock::now();
        mDuration = std::chrono::duration_cast<
            std::chrono::duration<double, std::ratio<1, 1000>>>(end - mStart);
        mStart = end;
    };

    /// string value of last duration "10ms"
    std::string str() const
    {
        return std::to_string(ms()) + "ms";
    };

    /// returns time in seconds
    double s() const
    {
        return std::chrono::duration_cast<
            std::chrono::duration<double, std::ratio<1, 1>>>(mDuration)
            .count();
    };

    /// returns time in miliseconds
    double ms() const
    {
        return std::chrono::duration_cast<
            std::chrono::duration<double, std::ratio<1, 1000>>>(mDuration)
            .count();
    };

    /// start new lap and returns time of the last one as string (format: 10ms)
    std::string lap_str()
    {
        lap();
        return str();
    }
};

} // namespace kr
