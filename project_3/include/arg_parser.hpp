#ifndef __arg_parser_cpp__
#define __arg_parser_cpp__

#include <string>
#include <iostream>
#include "penningTrap.hpp"

/**
 * @brief Struct to hold command-line arguments
 */
struct Args
{
    unsigned int n_steps = 4000;    ///< Number of steps for Penning trap simulation.
    int seed = 1234;                ///< See for random positions of Penning trap.
    double T = 50.0;                ///< Time duration of Penning trap simulation.
    int n_particles = 2;            ///< Number of particles in Penning trap.
    bool problem_8 = false;         ///< For running problem 8.
    bool problem_9 = false;         ///< For running problem 9.
    bool interacting = true;        ///< Whether the Penning trap includes interactions or not.
    double amplitude = 0.1;         ///< Amplitude in time-dependent potential.
    double freq_min = 0.2;          ///< Minimum frequency in time-dependent potential.
    double freq_max = 2.5;          ///< Maximum frequency in time-dependent potential.
    int n_freq = 10;                ///< Number of frequencies to run the simulation.
};

/**
 * @brief Function for parsing command-line arguments
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Args struct containing the parsed values
 */
Args parse_args(int argc, char *argv[]);

#endif