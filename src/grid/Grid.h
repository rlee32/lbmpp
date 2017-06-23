#pragma once

// Holds all of the coarsest cells in the domain in row-major order.

#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <omp.h>

#include "../cell/Cell.h"
#include "GridLevel.h"
#include "../config/Config.h"

class Grid
{
public:
  Grid();
  Grid(const Config&);

  void iteration(std::size_t level);
  std::size_t active_cells() const;
  std::vector<Cell>& get_cells(std::size_t index)
    { return m_levels[index].get_cells(); }
  GridLevel& get_level(std::size_t index)
    { return m_levels[index]; }
  //
  void initialize( std::size_t cell_count_x, std::size_t cell_count_y,
    double rho0, double u0, double v0,
    double nu, double nuc,
    std::array<char, 4> sides, std::array<char, 4> bc, double U,
    double relax_model, double vc_model, bool experimental );
  void set_coarse_solution(
    double rho0, std::vector<double>& u, std::vector<double>& v );
  //
  std::size_t cell_count_x() { return m_cell_count[0]; }
  std::size_t cell_count_y() { return m_cell_count[1]; }
  // Post-processing functions.
  double max_mag() const;
  double min_mag() const;
  double max_rho() const;
  double min_rho() const;
  double mag( std::size_t level, std::size_t cell_index ) const;
  void reconstruct_macro();
  // Operators
  GridLevel& operator[](std::size_t i) { return m_levels[i]; }

private:
  static constexpr int MAX_LEVELS = 32;
  std::array<GridLevel, MAX_LEVELS> m_levels;
  double m_relax_model;
  double m_vc_model;
  std::array<int, 2> m_cell_count; // coarsest level cell dimension.

  void experimental_initialize();
};
