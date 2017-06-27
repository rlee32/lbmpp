// Author: Robert Lee
// Email: rlee32@gatech.edu

// This version simply runs to the specified timesteps and writes out results.
// No visualization.

#include <cstdlib>
#include <iostream>

#include "Log.h"
#include "sim/Simulator.h"
#include "Stopwatch.h"

int main(int /*argc*/, char** /*argv*/)
{
    const std::string configFilename("settings");
    const Config config(configFilename);
    Grid grid(config);
    Simulator sim(config, grid);
    Stopwatch stopwatch;
    Log("Running ", config.timesteps, " timesteps.");
    int timestep = 0;
    stopwatch.start();
    while (timestep <= config.timesteps)
    {
        sim.iteration();
        if (timestep % config.display_interval == 0)
        {
            const double speed = static_cast<double>(timestep) / stopwatch.stopSeconds();
            Log("Average speed: ", speed, " iterations / second");
            if (config.picset)
            {
                sim.output_picset_field(timestep / config.display_interval);
            }
        }
        ++timestep;
    }
    Log("Simulation finished!");
    sim.output_solution();
    return EXIT_SUCCESS;
}
