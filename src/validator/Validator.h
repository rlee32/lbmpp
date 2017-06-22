#pragma once

#include <cmath>
#include <vector>

namespace Validation
{

// Returns a negative value for error, otherwise the L2 norm difference
// between the two input vectors.
inline double L2Norm(const std::vector<double>& v1, const std::vector<double>& v2)
{
    if (v1.size() != v2.size())
    {
        return -1;
    }
    double sumsq = 0;
    for (int i = 0; i < v1.size(); ++i)
    {
        const double dv = v1[i] - v2[i];
        sumsq += dv * dv;
    }
    return std::sqrt(sumsq);
}

} // namespace Validation

