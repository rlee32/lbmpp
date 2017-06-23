#include "Config.h"

Config::Config(const std::string& filename)
{
    std::cout << "Reading config file: " << filename << std::endl;
    bc[0] = 'w';
    bc[1] = 'w';
    bc[2] = 'w';
    bc[3] = 'w';
    face_order[0] = "bottom";
    face_order[1] = "right";
    face_order[2] = "top";
    face_order[3] = "left";
    face_order_char[0] = 'b';
    face_order_char[1] = 'r';
    face_order_char[2] = 't';
    face_order_char[3] = 'l';
    cell_count[0] = 0;
    cell_count[1] = 0;
    readSettings(filename);
    std::string grid_string = "G" + std::to_string((long long)cell_count[0]);
    if(cell_count[0] != cell_count[1])
    {
        grid_string += "x" + std::to_string((long long)cell_count[1]);
    }
    const std::string mach_string = "M" + std::to_string((long long)(M*1000.0));
    const std::string timesteps_string = "T" + std::to_string((long long)(timesteps/1000));
    const std::string relax_model_string = "RM" + std::to_string((long long)(relax_model));
    const std::string vc_model_string = "VCM" + std::to_string((long long)(vc_model) );
    const std::string nucf_string = "VCF" + std::to_string((long long)round((nuc/nu)*10.0));
    const std::string re_string = "Re" + std::to_string((long long)round(Re));
    output_suffix = grid_string
        + "_" + mach_string
        + "_" + timesteps_string
        + "_" + relax_model_string
        + "_" + vc_model_string
        + "_" + nucf_string
        + "_" + re_string;
}

void Config::readSettings(const std::string& filename)
{
    std::ifstream fileStream(filename);
    std::string loadInitialFieldFromFile = "no";
    if(fileStream.is_open())
    {
        std::string line;
        while(getline(fileStream, line))
        {
            std::istringstream stringStream(line);
            std::string parameter;
            // If content available
            if(stringStream >> parameter)
            {
                if (parameter[0] == '#')
                {
                    continue;
                }
                else if(parameter == "cell_count")
                {
                    stringStream >> cell_count[0];
                    stringStream >> cell_count[1];
                }
                else if(parameter == "M")
                {
                    stringStream >> M;
                }
                else if(parameter == "L")
                {
                    stringStream >> L;
                }
                else if(parameter == "Re")
                {
                    stringStream >> Re;
                }
                else if(parameter == "vc_model")
                {
                    stringStream >> vc_model;
                }
                else if(parameter == "nucf")
                {
                    stringStream >> nucf;
                }
                else if(parameter == "relax_model")
                {
                    stringStream >> relax_model;
                }
                else if(parameter == "timesteps")
                {
                    stringStream >> timesteps;
                }
                else if(parameter == "refinement")
                {
                    stringStream >> refinement;
                }
                else if(parameter == "rho0")
                {
                    stringStream >> rho0;
                }
                else if(parameter == "u0")
                {
                    stringStream >> u0;
                }
                else if(parameter == "v0")
                {
                    stringStream >> v0;
                }
                else if(parameter == "u0file")
                {
                    stringStream >> u0file;
                }
                else if(parameter == "v0file")
                {
                    stringStream >> v0file;
                }
                else if(parameter == "M0")
                {
                    stringStream >> M0;
                }
                else if(parameter == "display_interval")
                {
                    stringStream >> display_interval;
                }
                else if(parameter == "picset")
                {
                    stringStream >> picset;
                }
                else if(parameter == "experimental")
                {
                    stringStream >> experimental;
                }
                else if(parameter == "load_from_file")
                {
                    stringStream >> loadInitialFieldFromFile;
                }
                for(int i = 0; i < 4; ++i)
                {
                    if(parameter == face_order[i])
                    {
                        stringStream >> bc[i];
                    }
                }
            }
        }
        U = M / sqrt(3.0);
        nu = U * L / Re;
        nuc = (vc_model != 0) ? nucf * nu : 0; // counteracting viscosity.
        tau = 3.0 * (nu + nuc) + 0.5;
        omega = 1.0 / tau;
        if (loadInitialFieldFromFile == "yes")
        {
            if (not u0file.empty() and not v0file.empty())
            {
                std::cout << "Reading initial field from files: "
                    "\n\tu-velocity: " << u0file
                    << "\n\tv-velocity: " << v0file
                    << std::endl;
                read_coarse_solution();
            }
            else
            {
                std::cout << "Attempted to read initial field from files"
                    ", but one or more filenames were missing." << std::endl;
            }
        }
        std::cout << "Successfully read config file." << std::endl;
    }
    else
    {
        std::cerr << "Unable to open config file: " << filename << std::endl;
    }
}

// Read in the components of a row-major coarse grid
//  and interpolate values to current grid.
// filename: name of file where each line is a row of cells.
//  Example: 256, 128 is represented by 256 columns, 128 lines.
// Returns x_cells.
int Config::read_coarse_field(std::string filename, std::vector<double>& phi,
  double scale)
{
    int x_cells = 0;
    std::ifstream myfile (filename);
    if (myfile.is_open())
    {
        std::string line;
        bool first_line = true;
        while ( getline (myfile,line) )
        {
            std::istringstream iss(line);
            double value = 0;
            while(iss >> value)
            {
                phi.push_back( scale*value );
            }
            if(first_line)
            {
                x_cells = phi.size();
                first_line = false;
            }
        }
        myfile.close();
        if (phi.size() % x_cells != 0)
        {
          std::cout << "Error! Read " << phi.size() << " total cells, but x cells = "
            << x_cells << std::endl;
        }
        else
        {
          std::cout << "Read in " << filename
            << " (" << x_cells << "x" << phi.size()/x_cells << ")" << std::endl;
        }
      }
      else
      {
        std::cerr << "Unable to open file: " << filename << std::endl;
      }
      return x_cells;
}

void Config::interpolate_field(int source_x_cell_count, std::vector<double>& source,
  int target_c_cell_count, int target_y_cell_count, std::vector<double>& target)
{
  // Assume cell-centered fields.
  // Assume uniformly-sized square cells.
  // Target indices are converted to source indices, then bilinear interpolation
  //  is applied (as on Wikipedia page for bilinear interpolation).

  // Converting target indices to source indices for interpolation.
  double x_convert = ((double)source_x_cell_count) / ((double)target_c_cell_count);
  int source_y_cell_count = source.size() / source_x_cell_count;
  double y_convert = ((double)source_y_cell_count) / ((double)target_y_cell_count);
  for(int j = 0; j < target_y_cell_count; ++j)
  {
    double y = ( 0.5 + j ) * y_convert;
    int jj = ceil(y - 0.5);
    double dy = y - floor(y);
    if (jj >= source_y_cell_count)
    {
      jj = source_y_cell_count - 1;
      dy = 1.0;
    }
    if (jj <= 0)
    {
      jj = 1;
      dy = 0;
    }
    for(int i = 0; i < target_c_cell_count; ++i)
    {
      double x = (0.5 + i) * x_convert;
      int ii = ceil( x - 0.5 );
      double dx = x - floor(x);
      if (ii >= source_x_cell_count)
      {
        ii = source_x_cell_count - 1;
        dx = 1.0;
      }
      if (ii <= 0)
      {
        ii = 1;
        dx = 0;
      }
      // serial index for upper-right-most point
      int si = ii + jj*source_x_cell_count;
      double f11 = source[si - 1 - source_x_cell_count];
      double f12 = source[si - 1];
      double f21 = source[si - source_x_cell_count];
      double f22 = source[si];
      target.push_back(
        f11*(1-dx)*(1-dy)
        + f21*(dx)*(1-dy)
        + f12*(1-dx)*(dy)
        + f22*(dx)*(dy) );
    }
  }
}

// Read in the complete macroscopic state rho, u, and v velocity components
//  of a row-major coarse grid and interpolate values to current grid.
void Config::read_coarse_solution()
{
    std::vector<double> u_source;
    std::vector<double> v_source;
    std::vector<int> x_cells_candidates;
    std::vector<int> total_cells_candidates;
    x_cells_candidates.push_back(read_coarse_field(u0file, u_source, M / M0));
    x_cells_candidates.push_back(read_coarse_field(v0file, v_source, M / M0));
    int x_cells = *( max_element(
    x_cells_candidates.begin(), x_cells_candidates.end()) );
    total_cells_candidates.push_back( u_source.size() );
    total_cells_candidates.push_back( v_source.size() );
    int total_cells = *( max_element(
    total_cells_candidates.begin(), total_cells_candidates.end()) );
    int y_cells = total_cells / x_cells;
    if ( y_cells*x_cells != total_cells )
    {
    std::cout << "Error! Cell dimensions not consistent with total cells: "
      << x_cells << "x" << y_cells << " != " << total_cells << std::endl;
    return;
    }
    interpolate_field(x_cells, u_source,
    cell_count[0], cell_count[1], u0field);
    interpolate_field(x_cells, v_source,
    cell_count[0], cell_count[1], v0field);
}


