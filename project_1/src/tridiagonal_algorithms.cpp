
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>

std::vector<double> general_algorithm(std::vector<double> a,
                                     std::vector<double> b,
                                     std::vector<double> c,
                                     std::vector<double> g)
{

    // Initialize vectors:
    int n = b.size();

    std::vector<double> b_tilde(n);
    std::vector<double> g_tilde(n);
    std::vector<double> solution(n + 2);

    b_tilde[0] = b[0];
    g_tilde[0] = g[0];

    // Find b_tilde and g_tilde algorithmically:
    for (int i = 1; i < n; i++)
    {   
        // NOTE: Index a with (i-1) instead of (i), as a[0] = a_1 and so on...
        double temp = a[i - 1] / b_tilde[i - 1];        // Temporary variable for -n FLOPS
        b_tilde[i] = b[i] - temp * c[i - 1];
        g_tilde[i] = g[i] - temp * g_tilde[i - 1];
    }

    // Use these to find solution vector v;
    solution[n - 1] = g_tilde[n - 1] / b_tilde[n - 1];

    for (int i = n - 2; i >= 1; i--)
    { 
        solution[i] = (g_tilde[i] - c[i] * solution[i + 1]) / b_tilde[i];
    }

    return solution;
}

std::vector<double> special_algorithm(std::vector<double> g)
{

    // Initialize vectors:
    int n = g.size();

    std::vector<double> g_tilde(n);
    std::vector<double> b_tilde(n);
    std::vector<double> solution(n + 2);

    g_tilde[0] = g[0];
    b_tilde[0] = 2;

    // Find g_tilde algorithmically:
    for (int i = 1; i < n; i++)
    {
        b_tilde[i] = 1.0 + 1.0/i;
        g_tilde[i] = g[i] + g_tilde[i - 1] / b_tilde[i - 1];
    }

    // Use these to find solution vector v;
    solution[n - 1] = g_tilde[n - 1] / b_tilde[n - 1];

    for (int i = n - 2; i >= 1; i--)
    {
        solution[i] = (g_tilde[i] + solution[i + 1]) / b_tilde[i];
    }

    return solution;
}