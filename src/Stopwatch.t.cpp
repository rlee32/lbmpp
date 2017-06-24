#include <gtest/gtest.h>

#include "Stopwatch.h"

TEST(Stopwatch, ctor)
{
    Stopwatch stopwatch;
    stopwatch.start();
    double secondsElapsed = stopwatch.stopSeconds();
}

