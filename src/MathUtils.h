#pragma once

#include <limits>

namespace MathUtils {

inline bool Equals(const double a, const double b)
{
    constexpr double Tolerance = std::numeric_limits<double>::epsilon();
    return std::abs(a - b) <= Tolerance;
}

} // namespace MathUtils
