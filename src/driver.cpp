// Author: Robert Lee
// Email: rlee32@gatech.edu

// This version simply runs to the specified timesteps and writes out results.
// No visualization.

#include <cstdlib>
#include <iostream>

#include "sim/Simulator.h"
#include "Stopwatch.h"

int main(int /*argc*/, char** /*argv*/)
{
    const std::string configFilename("settings");
    const Config config(configFilename);
    Grid grid(config);
    Simulator sim(config, grid);
    Stopwatch stopwatch;
    std::cout << "Running " << config.timesteps << " timesteps." << std::endl;
    int timestep = 0;
    stopwatch.start();
    while (timestep <= config.timesteps)
    {
        sim.iteration();
        if (timestep % config.display_interval == 0)
        {
            const double speed = static_cast<double>(timestep) / stopwatch.stopSeconds();
            std::cout << "Average speed: " << speed << " iterations / second" << std::endl;
            if (config.picset)
            {
                sim.output_picset_field(timestep / config.display_interval);
            }
        }
        ++timestep;
    }
    std::cout << "Simulation finished!" << std::endl;
    sim.output_solution();
    return EXIT_SUCCESS;
}
