
#ifndef __solver_hpp__
#define __solver_hpp__

#include "penningTrap.hpp"
#include <string>

/**
 * @brief Class to solve evolve the Penning trap using a variety of possible numerical integration methods.
 * 
 */
class Solver
{

public:

    /**
     * @brief Construct a new Solver object.
     * 
     * @param @ref PenningTrap "PenningTrap" object.
     */
    Solver(PenningTrap &trap);

    /**
     * @brief Evolves the particles inside the Penning trap in time using the Runge-Kutta 4 method.
     * 
     * @param dt Time step.
     * @param t Time [\f$\mu s\f$] (microseconds)
     */
    void evolve_RK4(double dt, double t);
    
    /**
     * @brief Evolves the particles inside the Penning trap in time using the Forward Euler method.
     * 
     * @param dt Time step.
     * @param t Time [\f$\mu s\f$] (microseconds)
     */
    void evolve_FE(double dt, double t);

    /**
     * @details Evolves the system using a method of choice, default is Runge-Kutta 4.
     * 
     * @param T:        Total time evolution [\f$ \mu s\f$] (microseconds). 
     * @param n_steps:  Total number of time points.         
     * @param method:   Type of solver.
     */
    void evolve(double T, int n_steps, std::string method="RK4");

    /**
     * @brief Saves the computed evolution of the Penning trap.
     * 
     */
    void save(std::string additional_info="", std::string outdir="out/", double T=50);
    
private:
    PenningTrap &trap; ///< See @ref PenningTrap "PenningTrap".

    std::vector<std::string> possible_solvers = {"RK4", "FE"}; ///< Current possible solvers.
    using evolver = void (Solver::*)(double, double); 
    std::vector<evolver> evolvers = {&Solver::evolve_RK4, &Solver::evolve_FE}; ///< To index the current solver method
    std::string current_method; ///< To keep track of what solver is (was) being used. Used in Solver::save

    arma::cube particles_positions; ///< 3D array of particle positions (particle,time,3)
    arma::cube particles_velocities; ///< 3D array of particle positions (particle,time,3)
    arma::cube particles_inside_outside; ///< Whether particles are inside Penning trap or not at a given time index (particle,time,0 or 1).

    bool evolved = false; ///< To keep track of whether or not the Penning trap has been evolved in time.
    std::vector<double> div_masses;  ///< To compute 1/m[i] once, instead of at (e.g.) every k in RK4.
};

#endif
