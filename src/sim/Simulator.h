#pragma once

// Contains settings for the simulation.

#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>

#include "../grid/Grid.h"
#include "../grid/GridLevel.h"
#include "../config/Config.h"

// For post-processing purposes.
typedef struct CellData
{
  double u,v;
  double x,y;
  CellData():u(0),v(0),x(0),y(0){}
} CellData;

class Simulator
{
public:
    Simulator(const Config&, Grid&);
    void iteration(); // performs one time step.

    void output_solution();
    void output_picset_field(int k);

    const std::string outputFolder{"output/"};

    const Config& m_config;
    Grid& m_grid;

private:
  // Postprocessing
  void output_coarse_field();
  void get_data( Cell& cell,
    std::vector<CellData>& data, char side,
    double xstart, double ystart, double dim );
  void produce_centerline_y( std::vector<CellData>& side1,
    std::vector<CellData>& side2, std::vector<CellData>& center);
  void produce_centerline_x( std::vector<CellData>& side1,
    std::vector<CellData>& side2, std::vector<CellData>& center);
  void centerline2file( std::vector<CellData>& center );
  void print_centerlines(
    std::vector<CellData>& centerx, std::vector<CellData>& centery );
  void centerline_x(
    std::vector<CellData>& left_values, std::vector<CellData>& right_values );
  void centerline_y(
    std::vector<CellData>& top_values, std::vector<CellData>& bottom_values );
  void output_centerlines();
};

