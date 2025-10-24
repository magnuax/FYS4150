#ifndef __penningTrap_hpp__
#define __penningTrap_hpp__

#include <armadillo>
#include <vector>
#include <functional>

/**
 * @brief Representation of a particle with charge @ref q "q" and mass @ref m "m" located at @ref r "r" with a velocity @ref v "v". 
 * 
 */
class Particle{

    friend class PenningTrap; ///< The particle will be located inside the Penning trap.
    friend class Solver;
    
private:
    
    const double q;           ///< Electric charge [\f$ e \f$] (elementary charge).
    const double m;           ///< Mass [\f$ u \f$] (atomic mass unit).
    arma::vec r;            ///< Position [\f$ \mu m \f$] (micrometer).
    arma::vec v;            ///< Velocity [\f$ \mu m/(\mu s)  \f$] (micrometer per microsecond).
    
public:

    /**
     * @brief Construct a new Particle object and assigns the properties of a particle in the obvious way.
     * 
     * @param q Electric charge [\f$ e \f$] (elementary charge).
     * @param m Mass [\f$ u \f$] (atomic mass unit).
     * @param r Position [\f$ \mu m \f$] (micrometer).
     * @param v Velocity [\f$ \mu m \mu^{-1} s^{-1}  \f$] (micrometer per microsecond).
     */
    Particle(double q, double m, arma::vec r, arma::vec v);
};

/**
 * @brief Representation of the Penning trap model. Particles are trapped within the potential given by @anchor eq_1
 * \f[
 *  V(x,y,z) = \frac{V_0}{2d^2}\left(2z^2 - x^2 -y^2\right),    \tag{1}
 * \f]
 * with a static homogeneous \f$B\f$-field pointing in the \f$z\f$-direction.
 * 
 */
class PenningTrap{
    friend class Solver;

private:

    // Private variables:
    double k_e = 1.38935333 * 1e5; ///< Coulomb constant [\f$ u (\mu m)^3 (\mu s)^{-2}e^{-2} \f$] 
    double B_0;      ///< Magnetic field strength [\f$ u/(\mu s)e^{-1} \f$] (atomic mass unit per microsend per electric charge).
    double V_0;      ///< Applied potential [\f$ u/(\mu s)^2e^{-1} \f$] (atomic mass unit per microsend squared per electric charge).
    std::function<double(double)> V_0_callable; ///< Callable version of @ref V_0 "V_0" for adding time dependence.      
    double d;        ///< Characteristic dimension [\f$ \mu m \f$] (micrometer).

    int n;    ///< Number of particles inside Penning trap.
    std::vector<Particle> particles; ///< Collection of particles inside Penning trap.

public:

    // Public variables:
    bool interacting = true; ///< Whether or not the Penning trap includes interactions.
    bool zero_fields = false; ///< If true, turns of the fields outside r>d.

    ///// Constructors //// 
    /**
     * @brief Default construction of the Penning trap with \f$ B_0 = 9.65\cdot 10^1 u/(\mu s) e\f$, \f$ V_0 = 2.41\cdot 10^6 u(\mu m)^2/(\mu s)^2 e^{-1}\f$ and \f$ d=500\mu m\f$.
     * 
     * @param B_0 Magnetic field strength [\f$ u/(\mu s)e^{-1} \f$] (atomic mass unit per microsend per electric charge).
     * @param V_0 Applied potential [\f$ u/(\mu s)^2e^{-1} \f$] (atomic mass unit per microsend squared per electric charge).
     * @param d Characteristic dimension [\f$ \mu m \f$] (micrometer).
     */
    PenningTrap(double B_0=9.65*1e1, double V_0=2.41*1e6, double d=500);


    /**
     * @brief Default construction of the Penning trap with \f$ B_0 = 9.65\cdot 10^1 u/(\mu s) e\f$, \f$ V_0 = 2.41\cdot 10^6 u(\mu m)^2/(\mu s)^2 e^{-1}\f$ and \f$ d=500\mu m\f$.
     * 
     * @param V_0_func Applied potential [\f$ u/(\mu s)^2e^{-1} \f$] (atomic mass unit per microsend squared per electric charge).
     * @param B_0 Magnetic field strength [\f$ u/(\mu s)e^{-1} \f$] (atomic mass unit per microsend per electric charge).
     * @param d Characteristic dimension [\f$ \mu m \f$] (micrometer).
     */
    PenningTrap(std::function<double(double)> V_0_func, double B_0=9.65*1e1, double d=500);

    ///// Public Methods //// 
    /**
     * @param i i-th particle in the Penning trap.
     * @return The i-th @ref Particle "Particle" in the Penning trap. 
     */
    Particle& operator[](int i);

    /**
     * @brief The electric field at position \f$ r = (x,y,z)\f$.
     * @details Eq. @ref eq_1 "(1)" gives that the electric field is given by @anchor eq_2
     * \f[
     *      \vec{E} = -\nabla V = \frac{V_0}{d^2}\left(x, y, -2z\right).
     * \f]
     * 
     * @param r Position [\f$ \mu m \f$ ] (micrometer). 
     * @param t Time [\f$ \mu s\f$] (microseconds)
     * @return \f$ \vec{E}\f$
     */
    arma::vec E(const arma::vec &r, double t);

    /**
     * @brief The magnetic field at position \f$ r = (x,y,z)\f$, given by \f$\vec{B} = B_0 \hat{e}_z = (0,0,B_0)\f$.
     * 
     * @param r Position [\f$ \mu m \f$ ] (micrometer). 
     * @return \f$ \vec{B}\f$
     */
    arma::vec B(const arma::vec &r);

    /**
     * @brief The forced applied to a @ref Particle "Particle" by index, due to the other particles inside the Penning trap. 
     * 
     * @param i The n-th particle in the collection of particles inside the Penning trap.
     * @return arma::vec 
     */
    arma::vec F_interaction(int i);

    /**
     * @brief The total force applied to a @ref Particle "Particle" by index, due to both interactions and external E- and B-field
     * 
     * @param i The n-th particle in the collection of particles inside the Penning trap.
     * @param t Time \f$ \mu s\f$ (microseconds).
     * @return arma::vec 
     */
    arma::vec F(int i, double t);

    /**
     * @brief Adds a new particle to the Penning trap.
     * 
     * @param q Electric charge [\f$ e \f$] (elementary charge).
     * @param m Mass [\f$ u \f$] (atomic mass unit).
     * @param r Position [\f$ \mu m \f$] (micrometer).
     * @param v Velocity [\f$ \mu m \mu^{-1} s^{-1}  \f$] (micrometer per microsecond).
     */
    void add_particle(double q, double m, arma::vec r, arma::vec v);

    /**
     * @brief Adds a new particle to the Penning trap.
     * 
     * @param particle Instance of @ref Particle "Particle".
     */
    void add_particle(Particle &particle);

    /**
     * @brief Counts the number of particles located within |r| < d.
     *
     * @return int Number of particles inside the trap.
     */
    int count_inside();
};

#endif