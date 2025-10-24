
#ifndef __utils_hpp__
#define __utils_hpp__

#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>

/**
 * Creates a file `filename` with two comma-separated columns `x` and `y`.
 * @param x Values in column #1
 * @param y Values in column #2
 * @param filename Name of output file
 */
void write_to_file(std::vector<double> x, std::vector<double> y, std::string filename);

/**
 * The source-term of the Poisson equation `-u(x)'' = f(x)` we want to solve.
 * @param x
 * @return `f(x)`
 */
double source_term(double x);

/**
 * Known solution `u(x)` to the Poisson equation `-u(x)'' = f(x)` we want to solve.
 * @param x
 * @return `u(x)`
 */
double exact_solution(double x);

#endif