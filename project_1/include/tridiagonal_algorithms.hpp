
#ifndef __tridiagonal_algorithms_hpp__
#define __tridiagonal_algorithms_hpp__

#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>

/**
 * Applies the Thomas algorithm to find the vector v solving Av = g,
 * for a general (n x n) tridiagonal matrix A.
 * @param a Lower diagonal entries of A (length n - 1)
 * @param b Main  diagonal entries of A (length n)
 * @param c Upper diagonal entries of A (length n - 1)
 * @param g Right hand side (length n)
 * @return Solution vector, including boundaries (length n + 2)
 */
std::vector<double> general_algorithm(std::vector<double> a,
                                      std::vector<double> b,
                                      std::vector<double> c,
                                      std::vector<double> g);

/**
 * TODO: documentation
 */
std::vector<double> special_algorithm(std::vector<double> g);

#endif