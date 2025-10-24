#include "penningTrap.hpp"
#include <cmath>
#include <stdexcept>
#include <string>

//******** Particle ********
Particle::Particle(double q, double m, arma::vec r, arma::vec v) 
                : q(q), m(m), r(r), v(v){

                }

//******** Penning Trap ********

PenningTrap::PenningTrap(double B_0, double V_0, double d)
                        : n(0), B_0(B_0), V_0(V_0), d(d){
                            
                            // Creating callable version of V_0:
                            V_0_callable = [V_0](double /*t*/){
                                return V_0;
                            };
                        }

PenningTrap::PenningTrap(std::function<double(double)> V_0_func, double B_0, double d)
                        : n(0), B_0(B_0), d(d){
                            V_0_callable = V_0_func;
                        }

void PenningTrap::add_particle(Particle &particle){
    particles.push_back(particle);

    n ++; // The Penning trap now contains one more particle;
}

void PenningTrap::add_particle(double q, double m, arma::vec r, arma::vec v){
    Particle particle = Particle(q, m, r, v);
    add_particle(particle);
}

Particle& PenningTrap::operator[](int i){
    if(i >= n || i < 0){
        throw std::invalid_argument("Index " + std::to_string(i) + " must be within [0, " + std::to_string(n) + "].");
    }

    return particles[i]; // The i-th particle.
}


arma::vec PenningTrap::E(const arma::vec &r, double t){
    arma::vec E_field = arma::vec(3).fill(0);

    if (zero_fields){
        if (arma::norm(r) <= d){
            E_field += arma::vec({r[0], r[1], - 2*r[2]});
            E_field *= V_0_callable(t) / (d*d);
        }

        return E_field;
    }

    // Won't go here if if tests succeeds
    E_field = {r[0], r[1], - 2*r[2]};
    E_field *= V_0_callable(t) / (d*d);
    return E_field;
} 

arma::vec PenningTrap::B(const arma::vec &r){
    arma::vec B_field = arma::vec(3).fill(0);

    if (zero_fields){
        if (arma::norm(r) <= d){
            B_field += arma::vec({0, 0 , B_0});
        }

        return B_field;
    }
    
    // Won't go here if if tests succeeds
    B_field = {0, 0 , B_0};
    return B_field;
} 

arma::vec PenningTrap::F_interaction(int i){

    // Checking that i\in[0,n-1]
    if (i >= n || i < 0){
        throw std::invalid_argument("i = " + std::to_string(i) + "must be within [0, " + std::to_string(n) + "].");
    }
    arma::vec Force = arma::vec(3).fill(0);

    const double prefactor = k_e * particles[i].q;
    double norm;
    for (int j=0; j<n; j++){
        if (i != j){

            norm = arma::norm(particles[i].r - particles[j].r);
            Force += particles[j].q * (particles[i].r - particles[j].r) / std::pow(norm, 3);
        }
    }
    return prefactor * Force;
}

arma::vec PenningTrap::F(int i, double t)
{
    arma::vec E_field = E(particles[i].r, t);
    arma::vec B_field = B(particles[i].r);
    arma::vec F_coulomb(3, arma::fill::zeros);
    if (interacting){
        F_coulomb = F_interaction(i);
    }   

    // Total force on particle i:
    arma::vec F_tot = particles[i].q * (E_field + arma::cross(particles[i].v, B_field)) + F_coulomb;

    return F_tot;
}

int PenningTrap::count_inside()
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (arma::norm(particles[i].r) < d)
        {
            count++;
        }
    }
    return count;
}