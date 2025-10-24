#include "ODESolver.hpp"
#include <cctype>
#include <stdexcept>

///// SolverPenningTrap //////

///// Public methods ////// 
SolverPenningTrap::SolverPenningTrap(PenningTrap &penningTrap, std::string solver){


    // Converting solver to uppercase
    std::string solver_copy = solver;
    for (std::size_t i = 0; i < solver.size(); ++i) {
        unsigned char uc = static_cast<unsigned char>(solver[i]);
        solver[i] = static_cast<char>(std::toupper(uc));
    }

    if (std::find(possible_solvers.begin(), possible_solvers.end(), solver) != possible_solvers.end())
    {
        throw std::invalid_argument(solver_copy + "is not a recognized solver method. Possible solvers are...");
    }
}