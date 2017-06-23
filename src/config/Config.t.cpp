#include <gtest/gtest.h>

#include "Config.h"

TEST(Config, ctor)
{
    // Note that the config file path is relative to where the Makefile is.
    Config config("src/config/testconfig.txt");
    ASSERT_EQ(config.timesteps, 50000);
    ASSERT_EQ(config.cell_count[0], 125);
    ASSERT_EQ(config.cell_count[1], 250);
    ASSERT_EQ(config.cell_count[1], 250);
    ASSERT_EQ(config.Re, 1000);
    ASSERT_DOUBLE_EQ(config.M, 0.2);
    ASSERT_EQ(config.L, 129);
    ASSERT_EQ(config.relax_model, 3);
    ASSERT_EQ(config.vc_model, 0);
    ASSERT_EQ(config.nucf, 0);
    ASSERT_EQ(config.refinement, 0);
    ASSERT_EQ(config.bc[0], 'w'); // bottom
    ASSERT_EQ(config.bc[1], 'w'); // right
    ASSERT_EQ(config.bc[2], 'm'); // top
    ASSERT_EQ(config.bc[3], 'w'); // left
    ASSERT_EQ(config.rho0, 1);
    ASSERT_EQ(config.u0, 0);
    ASSERT_EQ(config.v0, 0);
    ASSERT_DOUBLE_EQ(config.M0, 0.1);
    ASSERT_STREQ(config.u0file.c_str(), "results/fields/u_G125_M100_T100_RM3_VCM0_VCF0_Re1000.dat");
    ASSERT_STREQ(config.v0file.c_str(), "results/fields/v_G125_M100_T100_RM3_VCM0_VCF0_Re1000.dat");
    ASSERT_EQ(config.display_interval, 1000);
    ASSERT_EQ(config.picset, 0);
    ASSERT_EQ(config.experimental, 0);
}

