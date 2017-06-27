#include <gtest/gtest.h>

#include "Cell.h"

TEST(Cell, ctor)
{
    Cell cell(0, 0, 0, nullptr, nullptr);
}

TEST(Cell, getU)
{
    std::vector<Cell> children, grandchildren;
    Cell cell(0, 0, 0, nullptr, &children);
    cell.refine(children, grandchildren);
    children[0].state.u = 0;
    children[1].state.u = 1;
    children[2].state.u = 2;
    children[3].state.u = 3;
    ASSERT_DOUBLE_EQ(cell.getU(0.75, 0.25), 0);
    ASSERT_DOUBLE_EQ(cell.getU(0.75, 0.75), 1);
    ASSERT_DOUBLE_EQ(cell.getU(1.00, 1.00), 1);
    ASSERT_DOUBLE_EQ(cell.getU(0.25, 0.25), 2);
    ASSERT_DOUBLE_EQ(cell.getU(0.00, 0.00), 2);
    ASSERT_DOUBLE_EQ(cell.getU(0.25, 0.75), 3);
}
