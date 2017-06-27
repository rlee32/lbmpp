#include "Centerlines.h"

Centerlines::Centerlines(const Grid&, const Data& y, const Data& x)
{
    for(const auto p : y)
    {
        Log(p);
    }
    for(const auto p : x)
    {
        Log(p);
    }
}

/*
void getLeftmostData()
{

}

// This is used when the centerline cuts through centerCells,
// as opposed to bordering cells.
// Gets u value at normalized position y on the vertical centerline.
// y is in [0, 1]
double uVerticalCenterline(
    const std::vector<const Cell*> centerCells,
    const Grid& m_grid,
    const double y)
{
    std::numeric_limits<double>::epsilon();
    if (y == 0.0)
    {
        // TODO: implement generic boundary value.
        return 0.0; // assuming wall.
    }
    else if (y == 1.0)
    {
        // TODO: implement generic boundary value.
        return m_config.U; // assuming lid.
    }
    else
    {

    }
    for (const auto cell : centerCells)
    {

        if (cell->active())
        {
            // right on centerline
            CellData cd;
            cd.u = cell.u();
            cd.v = cell.v();
            cd.x = 0.5;
            cd.y = (j+0.5)*dim;
            left_values.push_back(cd);
            right_values.push_back(cd);
        }
        else
        {
            // left of centerline
            get_data( cell(0), left_values, 'r', 0.5-0.5 * dim, j * dim, 0.5 * dim );
            get_data( cell(1), left_values, 'r', 0.5-0.5 * dim, (j + 0.5) * dim, 0.5 * dim );
            // right of centerline
            get_data( cell(2), right_values, 'l', 0.5, j * dim, 0.5 * dim );
            get_data( cell(3), right_values, 'l', 0.5, (j + 0.5) * dim, 0.5 * dim );
        }
    }

    for(int j = 0; j < m_config.cell_count[1]; ++j)
    {
        int ii = j*m_config.cell_count[0] + start_i;
      Cell& cell = m_grid[0][ii];
      if (cell.active())
      {
        // right on centerline
        CellData cd;
        cd.u = cell.u();
        cd.v = cell.v();
        cd.x = 0.5;
        cd.y = (j+0.5)*dim;
        left_values.push_back(cd);
        right_values.push_back(cd);
      }
      else
      {
        // left of centerline
        get_data( cell(0), left_values, 'r', 0.5-0.5*dim, j*dim, 0.5*dim );
        get_data( cell(1), left_values, 'r', 0.5-0.5*dim, (j+0.5)*dim, 0.5*dim );
        // right of centerline
        get_data( cell(2), right_values, 'l', 0.5, j*dim, 0.5*dim );
        get_data( cell(3), right_values, 'l', 0.5, (j+0.5)*dim, 0.5*dim );
      }
    }
}

// Usable on our tree dynamic grid.
// Assumes lid-driven cavity, with top surface as moving lid.
// Assumes square domain with square cells.
void Simulator::centerline_x(
  std::vector<CellData>& left_values, std::vector<CellData>& right_values)
{
  CellData bottom_wall;
  bottom_wall.u = 0;
  bottom_wall.v = 0;
  bottom_wall.x = 0.5;
  bottom_wall.y = 0;
  left_values.push_back(bottom_wall);
  right_values.push_back(bottom_wall);
  double dim = 1.0 / m_config.cell_count[1];

  // odd number of cells along the x-direction.
    const bool oddCellCount = m_config.cell_count[0] & 1;
  if (oddCellCount)
  {
    int start_i = m_config.cell_count[0] >> 1;
    for(int j = 0; j < m_config.cell_count[1]; ++j)
    {
      int ii = j*m_config.cell_count[0] + start_i;
      Cell& cell = m_grid[0][ii];
      if ( cell.active() )
      {
        // right on centerline
        CellData cd;
        cd.u = cell.u();
        cd.v = cell.v();
        cd.x = 0.5;
        cd.y = (j+0.5)*dim;
        left_values.push_back(cd);
        right_values.push_back(cd);
      }
      else
      {
        // left of centerline
        get_data( cell(0), left_values, 'r', 0.5-0.5*dim, j*dim, 0.5*dim );
        get_data( cell(1), left_values, 'r', 0.5-0.5*dim, (j+0.5)*dim, 0.5*dim );
        // right of centerline
        get_data( cell(2), right_values, 'l', 0.5, j*dim, 0.5*dim );
        get_data( cell(3), right_values, 'l', 0.5, (j+0.5)*dim, 0.5*dim );
      }
    }
  }
  // even number of cells along the x-direction.
  else
  {
    int right_i = m_config.cell_count[0] >> 1;
    int left_i = right_i - 1;
    // left of centerline
    for( int j = 0; j < m_config.cell_count[1]; ++j )
    {
      int ii = j * m_config.cell_count[0] + left_i;
      double xstart = left_i*dim;
      double ystart = j*dim;
      get_data( m_grid[0][ii], left_values, 'r', xstart, ystart, dim );
    }
    // left of centerline
    for( int j = 0; j < m_config.cell_count[1]; ++j )
    {
      int ii = j * m_config.cell_count[0] + right_i;
      double xstart = right_i*dim;
      double ystart = j*dim;
      get_data( m_grid[0][ii], right_values, 'l', xstart, ystart, dim );
    }
  }
  CellData top_wall;
  top_wall.u = m_config.U;
  top_wall.v = 0;
  top_wall.x = 0.5;
  top_wall.y = 1.0;
  left_values.push_back(top_wall);
  right_values.push_back(top_wall);
}


void Simulator::output_centerlines()
{
    std::vector<CellData> top;
    std::vector<CellData> bottom;
    std::vector<CellData> right;
    std::vector<CellData> left;
    centerline_x(left, right);
    centerline_y(top, bottom);
    std::vector<CellData> centerx;
    std::vector<CellData> centery;
    produce_centerline_y(top, bottom, centery);
    produce_centerline_x(left, right, centerx);
    print_centerlines(centerx, centery);
}

// Inherently sequential.
// Traverses children to get the data points closest to a particular edge of
//  this cell.
// cell: cell under current investigation.
// xstart, ystart: the x,y coordinates of the lower left corner of this cell.
// dim: dimension of current cell.
void Simulator::get_data( Cell& cell,
  std::vector<CellData>& data, char side,
  double xstart, double ystart, double dim )
{
  if ( cell.active() )
  {
    CellData cd;
    cd.u = cell.u();
    cd.v = cell.v();
    cd.x = xstart + 0.5*dim;
    cd.y = ystart + 0.5*dim;
    data.push_back( cd );
  }
  else
  {
    dim *= 0.5;
    double xnew = xstart;
    double ynew = ystart;
    switch ( side )
    {
      case 't': // top
        ynew += dim;
        get_data( cell(1), data, side, xnew, ynew, dim );
        xnew += dim;
        get_data( cell(3), data, side, xnew, ynew, dim );
        break;
      case 'b': // bottom
        get_data( cell(0), data, side, xnew, ynew, dim );
        xnew += dim;
        get_data( cell(2), data, side, xnew, ynew, dim );
        break;
      case 'l': // left
        get_data( cell(0), data, side, xnew, ynew, dim );
        ynew += dim;
        get_data( cell(1), data, side, xnew, ynew, dim );
        break;
      case 'r': // right
        xnew += dim;
        get_data( cell(2), data, side, xnew, ynew, dim );
        ynew += dim;
        get_data( cell(3), data, side, xnew, ynew, dim );
        break;
      case 'm': // middle should be taken care of outside
        break;
      default: break;
    }

  }
}

// Usable on our tree dynamic grid.
// Assumes lid-driven cavity, with top surface as moving lid.
// Assumes square domain with square cells.
void Simulator::centerline_y(
  std::vector<CellData>& top_values, std::vector<CellData>& bottom_values)
{
  CellData left_wall;
  left_wall.u = 0;
  left_wall.v = 0;
  left_wall.x = 0;
  left_wall.y = 0.5;
  top_values.push_back(left_wall);
  bottom_values.push_back(left_wall);
  double dim = 1.0 / m_config.cell_count[0];
  // odd
  if ( m_config.cell_count[1] & 1 )
  {
    int start_j = m_config.cell_count[1] / 2;
    for( int i = 0; i < m_config.cell_count[0]; ++i )
    {
      int ii = start_j * m_config.cell_count[0] + i;
      Cell& cell = m_grid[0][ii];
      if ( cell.active() )
      {
        // right on centerline
        CellData cd;
        cd.u = cell.u();
        cd.v = cell.v();
        cd.x = (i+0.5)*dim;
        cd.y = 0.5;
        top_values.push_back(cd);
        bottom_values.push_back(cd);
      }
      else
      {
        // below centerline
        get_data( cell(0), bottom_values, 't', i*dim, 0.5-0.5*dim, 0.5*dim );
        get_data( cell(2), bottom_values, 't', (i+0.5)*dim, 0.5-0.5*dim, 0.5*dim );
        // above centerline
        get_data( cell(1), top_values, 'b', i*dim, 0.5, 0.5*dim );
        get_data( cell(3), top_values, 'b', (i+0.5)*dim, 0.5, 0.5*dim );
      }
    }
  }
  // even
  else
  {
    int top_j = m_config.cell_count[1] >> 1;
    int bottom_j = top_j - 1;
    // below centerline
    for( int i = 0; i < m_config.cell_count[0]; ++i )
    {
      int ii = bottom_j * m_config.cell_count[0] + i;
      double xstart = i*dim;
      double ystart = bottom_j*dim;
      get_data( m_grid[0][ii], bottom_values, 't', xstart, ystart, dim );
    }
    // above centerline
    for( int i = 0; i < m_config.cell_count[0]; ++i )
    {
      int ii = top_j * m_config.cell_count[0] + i;
      double xstart = i*dim;
      double ystart = top_j*dim;
      get_data( m_grid[0][ii], top_values, 'b', xstart, ystart, dim );
    }
  }
  CellData right_wall;
  right_wall.u = 0;
  right_wall.v = 0;
  right_wall.x = 1.0;
  right_wall.y = 0.5;
  top_values.push_back(right_wall);
  bottom_values.push_back(right_wall);
}
// From side1 and side2 vector data, produces centerline data
// we assume 0.5 is the centerline for x or y
void Simulator::produce_centerline_y(std::vector<CellData>& side1,
        std::vector<CellData>& side2, std::vector<CellData>& center)
{
  CellData cd;
  cd.u = 0;
  cd.v = 0;
  cd.x = 0;
  cd.y = 0.5;
  while( not side1.empty() and not side2.empty() )
  {
    double x1 = side1.back().x;
    double x2 = side2.back().x;
    double y1 = side1.back().y;
    double y2 = side2.back().y;
    if( x1 == x2 )
    {
      cd.x = x1;
      if(y1 == y2)
      {
        cd.u = side1.back().u;
        cd.v = side1.back().v;
      }
      else
      {
        cd.u = ( side1.back().u + side2.back().u ) * 0.5;
        cd.v = ( side1.back().v + side2.back().v ) * 0.5;
      }
      center.push_back(cd);
      side1.pop_back();
      side2.pop_back();
    }
    else
    {
      double dx = x2 - x1;
      double dy = y2 - y1;
      double mag = sqrt( dx*dx + dy*dy );
      double dy1 = 0.5 - y1;
      double dx1 = dy1 / dy * dx;
      double mag1 = sqrt( dx1*dx1 + dy1*dy1 );
      double r2 = mag1 / mag;

      cd.x = ( 1.0 - r2 ) * x1 + r2 * x2;
      cd.u = ( 1.0 - r2 ) * side1.back().u + r2 * side2.back().u;
      cd.v = ( 1.0 - r2 ) * side1.back().v + r2 * side2.back().v;

      center.push_back(cd);
      if( x1 < x2 ) side1.pop_back();
      if( x2 < x1 ) side2.pop_back();
    }
  }
}
// From side1 and side2 vector data, produces centerline data
// we assume 0.5 is the centerline for x or y
void Simulator::produce_centerline_x(std::vector<CellData>& side1,
  std::vector<CellData>& side2, std::vector<CellData>& center)
{
  CellData cd;
  cd.u = 0;
  cd.v = 0;
  cd.x = 0.5;
  cd.y = 0;
  while( not side1.empty() and not side2.empty() )
  {
    double x1 = side1.back().x;
    double x2 = side2.back().x;
    double y1 = side1.back().y;
    double y2 = side2.back().y;
    if( y1 == y2 )
    {
      cd.y = y1;
      if( x1 == x2 )
      {
        cd.u = side1.back().u;
        cd.v = side1.back().v;
      }
      else
      {
        cd.u = ( side1.back().u + side2.back().u ) * 0.5;
        cd.v = ( side1.back().v + side2.back().v ) * 0.5;
      }
      center.push_back(cd);
      side1.pop_back();
      side2.pop_back();
    }
    else
    {
      double dx = x2 - x1;
      double dy = y2 - y1;
      double mag = sqrt( dx*dx + dy*dy );
      double dx1 = 0.5 - x1;
      double dy1 = dx1 / dx * dy;
      double mag1 = sqrt( dx1*dx1 + dy1*dy1 );
      double r2 = mag1 / mag;

      cd.y = ( 1.0 - r2 ) * y1 + r2 * y2;
      cd.u = ( 1.0 - r2 ) * side1.back().u + r2 * side2.back().u;
      cd.v = ( 1.0 - r2 ) * side1.back().v + r2 * side2.back().v;

      center.push_back(cd);
      if( y1 > y2 ) side1.pop_back();
      if( y2 > y1 ) side2.pop_back();
    }
  }
}

// outputs a file with centerline info. these are the line descriptions:
// x positions of y-centerline
// v values of y-centerline
// y positions of x-centerline
// u values of x-centerline
void Simulator::print_centerlines(std::vector<CellData>& centerx, std::vector<CellData>& centery)
{
    std::ofstream out;
    out.open(outputFolder + "centerlines_" + m_config.output_suffix + ".tsv");
    out.precision(std::numeric_limits<double>::digits10);
    std::vector<CellData>::iterator it = centery.begin();
    for (; it != centery.end(); ++it)
    {
        out << it->x << "\t";
    }
    out << std::endl;
    it = centery.begin();
    for (; it != centery.end(); ++it)
    {
        out << it->v << "\t";
    }
    out << std::endl;
    it = centerx.begin();
    for (; it != centerx.end(); ++it)
    {
        out << it->y << "\t";
    }
    out << std::endl;
    it = centerx.begin();
    for (; it != centerx.end(); ++it)
    {
        out << it->u << "\t";
    }
    out << std::endl;
    out.close();
}
*/

