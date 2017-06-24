#pragma once

// This class extracts, stores, and outputs velocity centerline data.
// The vertical centerline is u on the iso-middle-x line of the domain.
// The horizontal centerline is v on the iso-middle-y line of the domain.
// This is used in validation of lid-driven cavity flow simulations.

#include <string>
#include <vector>

#include "../grid/Grid.h"
#include "CellData.h"

class Centerlines
{
    using Data = std::vector<double>;
    struct Datum;
public:
    // y are the y-coordinates of points along the vertical centerline to probe.
    // x are the x-coordinates of points along the horizontal centerline to probe.
    Centerlines(const Grid&, const Data& y, const Data& x);
    // CSV format.
    // First line is u on the vertical centerline.
    // Second line is v on the horizontal centerline.
    bool write(const std::string& filename);
    const Data& getVertical() const;
    const Data& getHorizontal() const;

private:
    Data m_vertical;
    Data m_horizontal;

};
