#include "penningTrap.hpp"
#include "solver.hpp"
#include "arg_parser.hpp"
#include <iostream>
#include "utils.hpp"
#include <iomanip>
#include <chrono>
#include <filesystem>


void problem8(const Args &args){
    //// Problem 8
    arma::vec r1 = {20, 0, 20}; // [mu m]
    arma::vec v1 = {0, 25, 0}; // [mu m/mu s]
    arma::vec r2 = {25, 25, 0}; // [mu m]
    arma::vec v2 = {0, 40, 5}; // [mu m/mu s]

    /// Single particle 
    int T = 50; // [mu s]
    PenningTrap single_particle;
    single_particle.add_particle(1, 40, r1, v1);
    Solver single_solver(single_particle);
    single_solver.evolve(T, args.n_steps); // Uses RK4 by default
    single_solver.save();

    /// Two particles
    PenningTrap interacting_trap;
    interacting_trap.add_particle(1, 40, r1, v1);
    interacting_trap.add_particle(1, 40, r2, v2);
    Solver interacting_solver(interacting_trap);
    interacting_solver.evolve(T, args.n_steps); // Uses RK4 by default
    interacting_solver.save();

    PenningTrap non_interacting_trap;
    non_interacting_trap.add_particle(1, 40, r1, v1);
    non_interacting_trap.add_particle(1, 40, r2, v2);
    non_interacting_trap.interacting = false;
    Solver non_interacting_solver(non_interacting_trap);
    non_interacting_solver.evolve(T, args.n_steps); // Uses RK4 by default
    non_interacting_solver.save();
    
    // Single particle, various specific time steps n 
    std::vector<int> n = {4000, 8000, 16000, 32000};

    for (std::string method : {"RK4", "FE"}) 
    {
        for (int i=0; i<n.size(); i++){
            PenningTrap single_particle;
            single_particle.add_particle(1, 40, r1, v1);
            Solver single_solver_(single_particle);
            single_solver_.evolve(T, n[i], method); // Uses RK4 by default
            single_solver_.save();
        }
    }
}


void problem9(const Args &args)
{

    // Prepare filename 
    std::ostringstream f_oss;
    f_oss << std::fixed << std::setprecision(1) << args.amplitude;
    std::string f_str = f_oss.str();
    std::string tmp   = f_str;
    tmp.erase(std::remove(tmp.begin(), tmp.end(), '.'), tmp.end());

    std::string info = "n_escaped-p" + std::to_string(args.n_particles) + "-f" + tmp + "-RK4";
    if (args.interacting) {
        info += "-int";
    }
    else {
        info += "-nonint";
    }
    info += "-n" + std::to_string(args.n_steps);
    
    std::string outdir = "out/";
    std::string filename = outdir + info + ".h5";

    // Run simulations
    std::cout << "---------------------------------" << std::endl;
    std::cout << "Starting problem 9 with f = " << f_str << std::endl;
    std::cout << "---------------------------------" << std::endl;
    
    arma::mat data;
    std::string time_stamp = get_time_stamp();

    // Try to load existing data. If fails, create empty dataset.
    if (std::filesystem::exists(filename))
    {
        arma::hdf5_name data_hdf5 = arma::hdf5_name(filename, "escape_data");
        data.load(data_hdf5);

        std::cout << time_stamp << " - Found existing dataset " << filename << " with " << data.n_cols << " entries." << std::endl;
        std::cout << time_stamp << " - Loading and appending to this dataset." << std::endl;
    }
    else
    {
        std::cout << time_stamp << " - No existing dataset found." << std::endl;
        std::cout << time_stamp << " - Creating new dataset at " << filename << std::endl;
        data.set_size(2, 0);
    }

    // Run simulations each frequency:

    std::cout << time_stamp << " - Running simulations for f = " << f_str << " ..." << std::endl;
    
    arma::vec omega_values = arma::linspace(args.freq_min, args.freq_max, args.n_freq);
    for (double omega : omega_values)
    {
        time_stamp = get_time_stamp();
        bool exists = check_exists(data, omega);
        
        // If omega already in dataset, skip 
        if (exists)
        {
            std::cout << time_stamp << "\t\t Skipping omega=" << omega << " (already in dataset)" << std::endl;
            continue;
        }
        // Else, run simulation and append to dataset
        else
        {
            std::cout << time_stamp << "\t\t Running omega=" << omega << std::endl;
            int escape_count = single_run_problem9(omega, args);
            arma::vec new_col = {omega, static_cast<double>(escape_count)};
            data.insert_cols(data.n_cols, new_col);
        }

        
    }


    // Sort new dataset by the frequency values in column 0 
    arma::uvec sort_idx = arma::sort_index(data.row(0));
    data = data.cols(sort_idx);

    // Save back to file
    arma::hdf5_name data_hdf5 = arma::hdf5_name(filename, "escape_data", arma::hdf5_opts::replace);
    data.save(data_hdf5, arma::hdf5_binary);
}

int main(int argc, char *argv[])
{
    Args args = parse_args(argc, argv);

    if (args.problem_8) {
        problem8(args);
    }
    if (args.problem_9) {
        problem9(args);
    }

    return 0;
}
