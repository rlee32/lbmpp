// Author: Robert Lee
// Email: rlee32@gatech.edu

// This version simply runs to the specified timesteps and writes out results.
// No visualization.

#include <cstdlib>
#include <iostream>

#include "sim/Simulator.h"
#include "Stopwatch.h"

using namespace std;

int main(int argc, char ** argv)
{
  // First read inputs and instantiate control panel.
  Simulator sim("settings");

  Stopwatch stopwatch;

  // Solution loop.
  cout << "Running " << sim.get_timesteps() << " timesteps." << endl;
  // bool done = false;
  int k = 0;
  stopwatch.start();
  while ( k <= sim.get_timesteps() )
  {
    sim.iteration();
    if ( k % sim.get_display_interval() == 0 )
    {
      cout << "Average speed: " << static_cast<double>(k) / stopwatch.stopSeconds()
        << " iterations / second" << endl;
      if ( sim.do_picset() )
      {
        sim.output_picset_field(k/sim.get_display_interval());
      }
    }
    ++k;
  }

  if ( k >= sim.get_timesteps() ) sim.output_solution();

  cout << "Simulation finished!" << endl;

  return EXIT_SUCCESS;
}
