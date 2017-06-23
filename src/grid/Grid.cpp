#include "Grid.h"

using namespace std;

Grid::Grid() : m_relax_model(1), m_vc_model(0)
{
    m_cell_count[0] = 0;
    m_cell_count[1] = 0;
}

Grid::Grid(const Config& config)
{
    // Transfer parameters.
    m_relax_model = config.relax_model;
    m_vc_model = config.vc_model;
    m_cell_count = config.cell_count;

    // Compute numerical parameters for each grid level.
    double scale_increase = 1;
    for (size_t i = 0; i < MAX_LEVELS; ++i)
    {
        GridLevel* next = (i < MAX_LEVELS - 1)
            ? &m_levels[i+1] : nullptr;
        GridLevel* parent = (i > 0)
            ? &m_levels[i-1] : nullptr;
        m_levels[i].initialize(scale_increase, config.nu, config.nuc,
            config.face_order_char, config.bc, config.U,
            next, parent);
        scale_increase *= 2.0;
    }

    // Create and initialize coarse cells.
    // Boundary conditions are also defined here.
    Cell default_cell(config.rho0, config.u0, config.v0,
        &m_levels[0].get_cells(), &m_levels[1].get_cells());
    m_levels[0].create_coarse_grid(m_cell_count[0], m_cell_count[1], default_cell);

    if (config.experimental)
    {
        experimental_initialize();
    }
}


void Grid::experimental_initialize()
{
  // Testing refine operation.
  // m_levels[0].refine_all();
  // m_levels[0].refine_half( cell_count_x, cell_count_y );
  // m_levels[0].refine_three_parts( cell_count_x, cell_count_y );
  // m_levels[0].refine_three_parts_rotated( cell_count_x, cell_count_y );
  // m_levels[0].refine_three_parts_rotated_flipped( cell_count_x, cell_count_y );
  // cout << "Successful refinement" << endl;
  // m_levels[0].print_cell_status( cell_count_x, cell_count_y );
}


// void printdist(Cell& cell)
// {
//   // cout << "grid f: ";
//   // for(int i = 0; i < 8; ++i ) cout << cell.state.f[i] << " ";
//   // cout <<endl;
//   // cout << "grid b: ";
//   // for(int i = 0; i < 8; ++i ) cout << cell.state.b[i] << " ";
//   // cout <<endl;
// }
// void printneighbours(Cell& cell)
// {
//   cout << "n: ";
//   for(int i = 0; i < 8; ++i ) cout << cell.local.neighbours[i] << " ";
//   cout <<endl;
// }
// // static void printchildren(Cell& cell)
// // {
// //   cout << "children: ";
// //   for(int i = 0; i < 4; ++i ) cout << cell(i).local.me << " ";
// //   cout <<endl;
// // }
// static void printinterface(Cell& c)
// {
//   cout << "coal interface: ";
//   for(size_t i = 0; i < 8; ++i ) if(c.bc.coalesce[i]) cout << c.bc.coalesce[i] << " ";
//   cout <<endl;
// }
// const int ccc = 10;

void Grid::iteration( size_t level )
{
  // printchildren(m_levels[0].get_cell(84));
  // printchildren(m_levels[0].get_cell(85));
  // printchildren(m_levels[0].get_cell(94));
  // printchildren(m_levels[0].get_cell(95));
  // // printneighbours(m_levels[0].get_cell(94));
  // printneighbours(m_levels[1].get_cell(197));
  // printchildren(m_levels[0].get_cell(5));
  // printchildren(m_levels[0].get_cell(15));
  // printchildren(m_levels[0].get_cell(25));
  // printchildren(m_levels[0].get_cell(35));
  // printchildren(m_levels[0].get_cell(45));
  // printchildren(m_levels[0].get_cell(55));
  // printchildren(m_levels[0].get_cell(65));
  // printchildren(m_levels[0].get_cell(75));
  // printchildren(m_levels[0].get_cell(85));
  // printchildren(m_levels[0].get_cell(95));
  // printchildren(m_levels[0].get_cell(9));
  // printchildren(m_levels[0].get_cell(19));
  // printchildren(m_levels[0].get_cell(29));
  // printchildren(m_levels[0].get_cell(39));
  // printchildren(m_levels[0].get_cell(49));
  // printchildren(m_levels[0].get_cell(59));
  // printchildren(m_levels[0].get_cell(69));
  // printchildren(m_levels[0].get_cell(79));
  // printchildren(m_levels[0].get_cell(89));
  // printchildren(m_levels[0].get_cell(99));
  // printinterface(m_levels[0].get_cell(4));
  // printinterface(m_levels[0].get_cell(14));
  // printinterface(m_levels[0].get_cell(24));
  // printinterface(m_levels[0].get_cell(34));
  // printinterface(m_levels[0].get_cell(44));
  // printinterface(m_levels[0].get_cell(54));
  // printinterface(m_levels[0].get_cell(64));
  // printinterface(m_levels[0].get_cell(74));
  // printinterface(m_levels[0].get_cell(84));
  // printinterface(m_levels[0].get_cell(94));

  // cout << "child: " << (m_levels[0].get_cell(95))(1).local.me << endl; 
  // cout << "child: " << (m_levels[0].get_cell(95))(3).local.me << endl; 
  // printneighbours(m_levels[1].get_cell(1));
  // cout << "child: " << (m_levels[0].get_cell(5))(1).local.me << endl; 
  // cout << "Start level " << level << endl;
  // First determine if next level exists.
  bool go_to_next_level = false;
  if (level < MAX_LEVELS-1)
  {
    if ( m_levels[level+1].get_active_cells() > 0 ) go_to_next_level = true;
  }
  // Main recursive iteration.
  // cout << "Collide level " << level << endl;
  // printdist(m_levels[1].get_cell(ccc));
  m_levels[level].collide( m_relax_model, m_vc_model );
  // printdist(m_levels[1].get_cell(ccc));
  if ( go_to_next_level )
  {
    // cout << "Explode level " << level << endl;
    // printdist(m_levels[1].get_cell(ccc));
    m_levels[level].explode();
    // printdist(m_levels[1].get_cell(ccc));
    iteration( level+1 );
    iteration( level+1 );
  }
  // cout << "Stream level " << level << endl;
  // printdist(m_levels[1].get_cell(ccc));
  m_levels[level].stream();
  // printdist(m_levels[1].get_cell(ccc));
  // cout << "Coalesce level " << level << endl;
  // printdist(m_levels[1].get_cell(ccc));
  if ( go_to_next_level ) m_levels[level].coalesce();
  // printdist(m_levels[1].get_cell(ccc));
}

void Grid::initialize(size_t cell_count_x, size_t cell_count_y,
  double rho0, double u0, double v0,
  double nu0, double nuc0,
  std::array<char, 4> sides, std::array<char, 4> bc, double U,
  double relax_model_, double vc_model_, bool experimental_ )
{
  // Assign parameters.
  m_relax_model = relax_model_;
  m_vc_model = vc_model_;
  m_cell_count[0] = cell_count_x;
  m_cell_count[1] = cell_count_y;

  // Compute numerical parameters for each grid level.
  double scale_increase = 1;
  for (size_t i = 0; i < MAX_LEVELS; ++i)
  {
    GridLevel* next = ( i < MAX_LEVELS - 1 ) ? &m_levels[i+1] : nullptr;
    GridLevel* parent = ( i > 0 ) ? &m_levels[i-1] : nullptr;
    m_levels[i].initialize( scale_increase, nu0, nuc0,
      sides, bc, U,
      next, parent );
    scale_increase *= 2.0;
  }

  // Create and initialize coarse cells.
  // Boundary conditions are also defined here.
  Cell default_cell( rho0, u0, v0,
    &m_levels[0].get_cells(), &m_levels[1].get_cells() );
  m_levels[0].create_coarse_grid( cell_count_x, cell_count_y, default_cell );

  if (experimental_)
  {
      experimental_initialize();
  }
}

void Grid::set_coarse_solution(
  double rho0, vector<double>& u, vector<double>& v )
{
  vector<Cell>& c = m_levels[0].get_cells();
  // cout << c.size() << endl;
  for(size_t i = 0; i < c.size(); ++i)
  {
    c[i].reconstruct_distribution( rho0, u[i], v[i] );
  }
}

double Grid::max_mag() const
{
  double max = m_levels[0].max_mag();
  // cout << "max: " << max << endl;  
  for(size_t i = 1; i < MAX_LEVELS; ++i)
  {
    double test = m_levels[i].max_mag();
    if (test > max) max = test;
  }
  return max;
}

double Grid::min_mag() const
{
  double min = m_levels[0].min_mag();
  // cout << "min: " << min << endl;
  for(size_t i = 1; i < MAX_LEVELS; ++i)
  {
    double test = m_levels[i].min_mag();
    if (test < min) min = test;
  }
  return min;
}
double Grid::max_rho() const
{
  double max = m_levels[0].max_rho();
  for(size_t i = 1; i < MAX_LEVELS; ++i)
  {
    double test = m_levels[i].max_rho();
    if ( test > max and test > 0 ) max = test;
  }
  return max;
}

double Grid::min_rho() const
{
  double min = m_levels[0].min_rho();
  for(size_t i = 1; i < MAX_LEVELS; ++i)
  {
    double test = m_levels[i].min_rho();
    if ( test < min and test > 0 ) min = test;
  }
  return min;
}

double Grid::mag( size_t level, size_t cell_index) const
{
  return m_levels[level].mag(cell_index);
}

size_t Grid::active_cells() const
{
  size_t total = 0;
  for(size_t i = 0; i < MAX_LEVELS; ++i)
  {
    total += m_levels[i].get_active_cells();
  }
  // cout << total << endl;
  return total;
}

void Grid::reconstruct_macro()
{
  for(size_t i = 0; i < MAX_LEVELS; ++i) m_levels[i].reconstruct_macro();
}
