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
    const std::string configFilename("settings");
    const Config config(configFilename);
    Grid grid(config);
    Simulator sim(config, grid);
    int max_pixel_dim = (argc < 2) ? 800 : atoi(argv[1]);
    SolutionViewer sv(config, sim, max_pixel_dim);
    Stopwatch stopwatch;
    stopwatch.start();
    int timestep = 1;
    while (not sv.window.is_closed() and timestep <= config.timesteps)
    {
        sim.iteration();
        if (timestep % config.display_interval == 0)
        {
            sv.draw_velocity_magnitude(grid);
            sv.draw_status(timestep, stopwatch.stopSeconds());
            sv.display();
            if (config.picset)
            {
                sim.output_picset_field(timestep / config.display_interval);
            }
        }
        ++timestep;
    }
    std::cout << "Completed " << timestep << " timesteps." << std::endl;
    const bool complete = timestep > config.timesteps;
    if (complete)
    {
        std::cout << "Simulation finished! Press enter to continue." << std::endl;
        sim.output_solution();
        sv.save_image(sim.outputFolder + "mag_" + config.output_suffix + ".png" );
        std::cin.ignore(); // Let the final view linger.
    }
    return EXIT_SUCCESS;
}
