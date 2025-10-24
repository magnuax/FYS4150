#ifndef __utils_hpp__
#define __utils_hpp__

#include <armadillo>
#include <chrono>
#include <iomanip>
#include "penningTrap.hpp"
#include "solver.hpp"
#include "arg_parser.hpp"

/**
 * @brief Checks if a given omega value exists in dataset.
 * *
 * @param data Dataset with omega values in first column.
 * @param omega Frequency value to check for.
 */
bool check_exists(const arma::mat &data, double omega);

/**
 * @brief Gets the current time as a formatted string `[HH:MM:SS]`.
 *
 * @return Formatted time string.
 */
std::string get_time_stamp();

/**
 * @brief Runs a single simulation for problem 9 with given parameters.
 *
 * @param f Amplitude of the time-dependent potential.
 * @param omega Frequency of the time-dependent potential.
 * @param d Characteristic dimension of the Penning trap.
 * @param args Command-line arguments struct.
 * @return Number of particles that have escaped the trap.
 */
int single_run_problem9(double omega, const Args &args);

#endif