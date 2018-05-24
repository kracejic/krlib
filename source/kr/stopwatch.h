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
    /// Helper global singleton instance. Can be still used without it.
    static Stopwatch& global()
    {
        thread_local static std::unique_ptr<Stopwatch> instance;
        if (not instance)
            instance.reset(new Stopwatch());
        return *instance;
    };

    /// Marks start of the timer.
    void start()
    {
        mStart = std::chrono::high_resolution_clock::now();
    };

    /// Calculates duration from start and restarts the counter, returns ms.
    double lap()
    {
        auto end = std::chrono::high_resolution_clock::now();
        mDuration = std::chrono::duration_cast<
            std::chrono::duration<double, std::ratio<1, 1000>>>(end - mStart);
        mStart = end;
        return mDuration.count();
    };

    /// String value of last duration "10ms".
    std::string str() const
    {
        return std::to_string(ms()) + "ms";
    };

    /// Returns time in seconds.
    double s() const
    {
        return std::chrono::duration_cast<
            std::chrono::duration<double, std::ratio<1, 1>>>(mDuration)
            .count();
    };

    /// Returns time in miliseconds.
    double ms() const
    {
        return std::chrono::duration_cast<
            std::chrono::duration<double, std::ratio<1, 1000>>>(mDuration)
            .count();
    };

    /// Start new lap and returns time of the last one as string (format: 10ms).
    std::string lap_str()
    {
        lap();
        return str();
    }
};

} // namespace kr
