#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <chrono>

#include "utils.hpp"
#include "tridiagonal_algorithms.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::string executable_name = argv[0];
        std::cerr << "Error: Wrong number of input arguments.\n";
        std::cerr << "Usage: " << executable_name << " <step number> <filename>\n";
        return 1;
    }

    int steps = std::stod(argv[1]);          // (no. of steps in the FULL solution)
    std::string filename = argv[2];
    const double x0 = 0.0;
    const double x1 = 1.0;
    const double h = (x1 - x0) / steps;

    // Initialize and fill x-, v-, and g-vectors:
    std::vector<double> x(steps + 1);
    std::vector<double> g(steps + 1);
    x[0] = x0;
    g[0] = h * h * source_term(0);

    for (int i = 1; i <= steps; i++)
    {
        x[i] = x[i-1] + h;
        g[i] = h * h * source_term(x[i]);
    }


    // Apply Thomas algorithm to find solution v, and write to file:
    
    int n = x.size() - 2;               // ( matrix eq. does not include the boundaries )
    std::vector<double> a(n, -1.0);
    std::vector<double> b(n + 1, 2.0);
    std::vector<double> c(n, -1.0);

    auto t1 = std::chrono::high_resolution_clock::now();
    
    std::vector<double> v = general_algorithm(a, b, c, g);
    
    auto t2 = std::chrono::high_resolution_clock::now();
    double duration_seconds = std::chrono::duration<double>(t2 - t1).count();
    std::cout << "Elapsed time: " << duration_seconds << " s\n";

    // Comment out when timing algorithms:
    // write_to_file(x, v, filename);
}