// Author: Robert Lee
// Email: rlee32@gatech.edu

#include <cstdlib>
#include <iostream>

#include "viz/CImg.h"
#include "viz/SolutionViewer.h"
#include "sim/Simulator.h"
#include "Stopwatch.h"

using namespace cimg_library;

int main(int argc, char ** argv)
{
    Simulator sim("settings");
    int max_pixel_dim = (argc < 2) ? 800 : atoi(argv[1]);
    SolutionViewer sv(sim, max_pixel_dim);
    Stopwatch stopwatch;
    stopwatch.start();
    int timestep = 0;
    while (not sv.window.is_closed() and timestep <= sim.get_timesteps())
    {
        sim.iteration();
        if (timestep % sim.get_display_interval() == 0)
        {
            sv.draw_velocity_magnitude(sim.grid);
            sv.draw_status(timestep, sim, stopwatch.stopSeconds());
            sv.display();
            if (sim.do_picset())
            {
                sim.output_picset_field(timestep / sim.get_display_interval());
            }
        }
        ++timestep;
    }
    const bool complete = timestep == sim.get_timesteps();
    if (complete)
    {
        std::cout << "Simulation finished! Press enter to continue." << std::endl;
        sim.output_solution();
        sv.save_image(sim.outputFolder + "mag_"+sim.get_output_suffix()+".png" );
        std::cin.ignore(); // Let the final view linger.
    }
    return EXIT_SUCCESS;
}
