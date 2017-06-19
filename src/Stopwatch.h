#pragma once

// A C++ stopwatch
// Call start() to start counting, then stop() to get the elapsed time.
// If desired, the accuracy can be changed in the stop() method.
//
// Usage:
// 1: Instantiate.
// 2: Start counting.
// 3: Get the time elapsed.
//
// Stopwatch stopwatch;
// stopwatch.start();
// double secondsElapsed = stopwatch.stopSeconds();

#include <chrono>

class Stopwatch
{
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
public:
    void start();
    double stopMicroseconds() const;
    double stopSeconds() const;

private:
    template<typename Units>
    Units elapsed() const;

    TimePoint m_start;
};

inline void Stopwatch::start()
{
    m_start = Clock::now();
}

template <typename Units>
inline Units Stopwatch::elapsed() const
{
    return std::chrono::duration_cast<Units>(Clock::now() - m_start);
}

inline double Stopwatch::stopSeconds() const
{
    using Seconds = std::chrono::duration<double>;
    return elapsed<Seconds>().count();
}

inline double Stopwatch::stopMicroseconds() const
{
    using Microseconds = std::chrono::duration<double, std::micro>;
    return elapsed<Microseconds>().count();
}

