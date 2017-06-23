#pragma once

// Reads and has accessors for simulation settings.

#include <algorithm>
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

struct Config
{
    Config(const std::string& filename);

    // Run time control.
    int timesteps{0};
    // Solver parameters.
    bool refinement; // If true, enables solution-adaptive refinement.
    int relax_model;
    int vc_model;
    // Physical parameters.
    double Re; // Reynolds number.
    double M; // Mach number for characteristic velocity.
    double U; // characteristic velocity, used for velocity BC values.
    double nu; // lattice viscosity of the coarsest cells.
    double L;
    double tau; // relaxation time of the coarsest cells.
    double omega; // relaxation frequency of the coarsest cells.
    // Initial values.
    double rho0;
    double u0;
    double v0;
    std::string u0file;
    std::string v0file;
    std::vector<double> u0field;
    std::vector<double> v0field;
    double M0;
    // For the viscosity-counteracting approach. The buffer viscosity.
    double nuc;
    double nucf;
    // Boundary conditions
    std::array<char, 4> bc; // bottom, right, top, left
    std::string face_order[4];
    std::array<char, 4> face_order_char;
    // (coarsest-cell) grid dimensions
    std::array<int, 2> cell_count;
    // output control
    int display_interval;
    bool picset;
    std::string output_suffix;
    bool experimental;

private:
    void readSettings(const std::string& filename);
    void read_coarse_solution();
    int read_coarse_field(std::string filename, std::vector<double>& phi, double scale);
    // coarse input solution.
    void interpolate_field(int source_x_cells, std::vector<double>& source,
        int target_x_cells, int target_y_cells,
        std::vector<double>& target);
};

