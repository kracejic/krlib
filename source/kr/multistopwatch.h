#pragma once

#include <chrono>
#include <cmath>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>

#include "kr/string.h"

namespace kr
{


class MultiStopwatch
{
  public:
    /// Helper global singleton instance.
    static MultiStopwatch& global()
    {
        thread_local static std::unique_ptr<MultiStopwatch> instance;
        if (not instance)
            instance.reset(new MultiStopwatch());
        return *instance;
    };

    /// Start/reset the MultiStopwatch, start segment.
    void start(const std::string& name)
    {
        mStart = std::chrono::high_resolution_clock::now();
        mCurLapStart = mStart;
        watches.clear();
        watches.emplace_back(name, 0, 0);
    }

    /// Trigger start of new segment.
    void lap(const std::string& name)
    {
        auto end = std::chrono::high_resolution_clock::now();
        double duration_ms = std::chrono::duration_cast<
            std::chrono::duration<double, std::ratio<1, 1000>>>(
            end - mCurLapStart)
                                 .count();

        mCurLapStart = std::chrono::high_resolution_clock::now();
        watches.back().duration_ms = duration_ms;
        watches.emplace_back(name, 0, 0);
    }

    struct NamedWatch
    {
        NamedWatch(
            const std::string& _name, double _duration_ms, float _percentage)
            : name(_name)
            , duration_ms(_duration_ms)
            , percentage(_percentage){};
        std::string name{""};
        double duration_ms;
        float percentage;

        operator std::string() const
        {
            return str();
        }

        std::string str(size_t namepad = 15) const
        {
            return rpad(name, namepad) + " - " +
                kr::lpad(kr::format("%.2f%% - ", percentage), 10) +
                std::to_string(duration_ms) + "ms";
        }
    };

    /// End last lap and calculate time percentages. Returns the resulting
    /// times.
    const std::vector<NamedWatch>& finalize()
    {
        auto end = std::chrono::high_resolution_clock::now();

        double duration_ms = std::chrono::duration_cast<
            std::chrono::duration<double, std::ratio<1, 1000>>>(
            end - mCurLapStart)
                                 .count();
        watches.back().duration_ms = duration_ms;

        mWholeDuration_ms = std::chrono::duration_cast<
            std::chrono::duration<double, std::ratio<1, 1000>>>(end - mStart)
                                .count();

        for (auto& watch : watches)
        {
            watch.percentage = 100 * (watch.duration_ms / mWholeDuration_ms);
        }
        watches.emplace_back("@ALL", mWholeDuration_ms, 100.0f);
        return watches;
    }

    /// Access the current results.
    const std::vector<NamedWatch>& results()
    {
        return watches;
    }

  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> mStart;
    std::chrono::time_point<std::chrono::high_resolution_clock> mCurLapStart;

    double mWholeDuration_ms;
    std::vector<NamedWatch> watches;
};
} /* kr */
