
#include "solver.hpp"
#include <stdexcept>
#include <iterator> // To find index of element in list (see Solver::evolve)

Solver::Solver(PenningTrap &trap) : trap(trap) {}

void Solver::evolve_RK4(double dt, double t){
    // Temporary storage of positions and velocities.
    int N = trap.n;

    std::vector<arma::vec> r_original(N);
    std::vector<arma::vec> v_original(N);

    std::vector<arma::vec> k_r1(N), k_r2(N), k_r3(N), k_r4(N);
    std::vector<arma::vec> k_v1(N), k_v2(N), k_v3(N), k_v4(N);

    for (int n=0; n<N; n++){
        r_original[n] = trap[n].r;
        v_original[n] = trap[n].v;
    }

    // Compute k's (div_masses is computed when calling Solver::evolve())
    for (int n=0; n<N; n++){    // k_1
        k_r1[n] = dt * trap[n].v;
        k_v1[n] = dt * trap.F(n, t) * div_masses[n];
    }
    for (int n=0; n<N; n++){    // Updating the positions and velocities
        trap[n].v = v_original[n] + 0.5 * k_v1[n];
        trap[n].r = r_original[n] + 0.5 * k_r1[n];
    }

    for (int n=0; n<N; n++){     // k_2
        k_r2[n] = dt * (v_original[n] + 0.5 * k_v1[n]);
        k_v2[n] = dt * trap.F(n, t + 0.5 * dt) * div_masses[n];
    }
    for (int n=0; n<N; n++){    // Updating the positions and velocities
        trap[n].v = v_original[n] + 0.5 * k_v2[n];
        trap[n].r = r_original[n] + 0.5 * k_r2[n];
    }
    
    for (int n=0; n<N; n++){     // k_3
        k_r3[n] = dt *(v_original[n] + 0.5 * k_v2[n]);
        k_v3[n] = dt * trap.F(n, t + 0.5 * dt) * div_masses[n];
    }
    for (int n=0; n<N; n++){    // Updating the positions and velocities
        trap[n].v = v_original[n] + k_v3[n];
        trap[n].r = r_original[n] + k_r3[n];
    }
    
    for (int n=0; n<N; n++){     // k_4
        k_r4[n] = dt * (v_original[n] + k_v3[n]);
        k_v4[n] = dt * trap.F(n, t + dt) * div_masses[n];
    }

    // Update positions and velocities
    for (int n=0; n<N; n++){
        trap[n].v = v_original[n] + (1.0 / 6.0) * (k_v1[n] + 2 * k_v2[n] + 2 * k_v3[n] + k_v4[n]);
        trap[n].r = r_original[n] + (1.0 / 6.0) * (k_r1[n] + 2 * k_r2[n] + 2 * k_r3[n] + k_r4[n]);
    }
}

void Solver::evolve_FE(double dt, double t){

    std::vector<arma::vec> Forces(trap.n);
    for (int i=0; i<trap.n; i++){
        Forces[i] = trap.F(i, t);
    }
    for (int i=0; i<trap.n; i++){
        // Forward Euler update:
        trap[i].r += dt * trap[i].v;
        trap[i].v += dt * Forces[i] * div_masses[i];
    }
}


void Solver::evolve(double T, int n_steps, std::string method){
    
    /* 
        Checking what method to use and if it exists. Here, 'it' either becomes the pointer to the element where an equal element to 
        method exists in possible_solvers, or the next memory address after possible_solvers. If the latter, the method is not implemented
        (yet) and an error is thrown. 
    */ 
    std::vector<std::string>::iterator it =  std::find(
                                                possible_solvers.begin(),
                                                possible_solvers.end(),
                                                method); 

    bool method_exists =  it != possible_solvers.end();

    if(!method_exists){
        std::string possible_solvers_string = "[";
        for (int i=0; i<possible_solvers.size()-1; i++){
            possible_solvers_string += possible_solvers[i] + ", ";
        }
        possible_solvers_string += possible_solvers_string[possible_solvers.size()-1] + "]";

        throw std::invalid_argument("I don't know what " + method + " is. Possible solvers are: " + possible_solvers_string);
    }

    // The method does exist, and this is its index 
    int index_solver = std::distance(possible_solvers.begin(), it);
    evolver evolve_method = evolvers[index_solver]; // Becomes a pointer to the method RK4, FE, or ...
    current_method = possible_solvers[index_solver]; // To be used in e.g. Solver::save

    int numb_particles = trap.n;
    double dt = T / (n_steps - 1);

    // Setting up time array and 3D 'cube' of (particles, steps, 3)
    arma::vec t_vector = arma::linspace(0.0, T, n_steps);
    particles_positions = arma::cube(numb_particles, n_steps, 3);
    particles_velocities = arma::cube(numb_particles, n_steps, 3);

    // Initial conditions and 1/m vector filling:
    div_masses.resize(numb_particles);
    for (int n=0; n<numb_particles; n++){
        particles_positions.tube(n,0) = trap.particles[n].r;
        particles_velocities.tube(n,0) = trap.particles[n].v;
        div_masses[n] = 1.0 / trap[n].m;
    }

    for (int i=1; i<n_steps; i++){
        (this->*evolve_method)(dt, t_vector[i-1]); // Dereferencing the pointer evolve_method and calling to this object (FE or RK4).

        // Storing positions and velocities.
        for (int n=0; n<numb_particles; n++){
            particles_positions.tube(n,i) = trap.particles[n].r;
            particles_velocities.tube(n,i) = trap.particles[n].v;
        }
    }

    evolved = true; // The Penning trap has evolved in time.
}

void Solver::save(std::string additional_info, std::string outdir, double T){
    int n_steps = particles_positions.n_cols;
    arma::mat particles_time = arma::linspace(0.0, T, n_steps);
    
    std::string numb_particles = "p" + std::to_string(trap.n);
    std::string interacting = "nonint"; std::string time_steps = std::to_string(n_steps);
    
    if (trap.interacting){
        interacting = "int";
    }
    std::string identifier = numb_particles + "-" + current_method + "-" + interacting + "-n" + time_steps;
    std::string filename = outdir + additional_info + identifier + ".h5";


    arma::hdf5_name time_hdf5 = arma::hdf5_name(filename, "times");
    arma::hdf5_name positions_hdf5 = arma::hdf5_name(filename, "positions", arma::hdf5_opts::append);
    arma::hdf5_name velocities_hdf5 = arma::hdf5_name(filename, "velocities", arma::hdf5_opts::append);

    particles_time.save(time_hdf5, arma::hdf5_binary);
    particles_positions.save(positions_hdf5, arma::hdf5_binary);
    particles_velocities.save(velocities_hdf5, arma::hdf5_binary);

    std::cout << "Successfully stored " << filename << std::endl;
}