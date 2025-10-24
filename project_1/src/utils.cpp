
#include "utils.hpp"

void write_to_file(std::vector<double> x, std::vector<double> y, std::string filename)
{
    int prec = 14;

    std::ofstream ofile;
    ofile.open(filename);

    for (int i = 0; i < x.size(); i++)
    {
        ofile << std::setprecision(prec) << std::scientific << x[i]
              << ","
              << std::setprecision(prec) << std::scientific << y[i]
              << std::endl;
    }
    ofile.close();
}

double source_term(double x)
{
    return 100 * std::exp(-10 * x);
}

double exact_solution(double x)
{
    return 1 - (1 - std::exp(-10)) * x - std::exp(-10 * x);
}