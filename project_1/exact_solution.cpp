#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>

#include "utils.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::string executable_name = argv[0];
        std::cerr << "Error: Wrong number of input arguments.\n;";
        std::cerr << "Usage: " << executable_name << " <step number> <filename>\n";
        return 1;
    }
    
    int n_steps = atoi(argv[1]);
    std::string filename = argv[2];
    const double x0 = 0.0;
    const double x1 = 1.0;
    const double h  = (x1 - x0) / n_steps;

    std::vector<double> x_vector(n_steps + 1);
    std::vector<double> u_vector(n_steps + 1);
    x_vector[0] = x0;
    u_vector[0] = exact_solution(x0);

    for (int i = 1; i <= n_steps; i++)
    {
        x_vector[i] = x_vector[i - 1] + h;
        u_vector[i] = exact_solution(x_vector[i]);
    }


    write_to_file(x_vector, u_vector, filename);

    return 0;
}