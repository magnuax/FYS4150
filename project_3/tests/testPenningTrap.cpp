#include "penningTrap.hpp"
#include <cassert>

int test_Particle(){

    float eps = 1e-15;
    float q = 1.0;
    float m = 1.0072764665789;
    arma::vec r = {20, 0, 20};
    arma::vec v = {0, 25, 0};

    Particle particle = Particle(q, m, r, v);
    // Add tests for Particle class 

}

int test_PenningTrap(){

    float eps = 1e-15;
    float q = 1.0;
    float m1 = 1.0072764665789; float m2 = m1*2;
    arma::vec r = {20, 0, 20};
    arma::vec v = {0, 25, 0};

    Particle particle = Particle(q, m1, r, v);

    PenningTrap penningTrap = PenningTrap();
    penningTrap.add_particle(particle);
    penningTrap.add_particle(q, m2, r, v);


    // Add tests for Penning trap class 
}