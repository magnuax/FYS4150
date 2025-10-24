#include <armadillo>
#include "penningTrap.hpp"
#include <string>

class SolverPenningTrap{

private:

    std::string type_of_solver;
    std::vector<std::string> possible_solvers = {"RK4", "FORWARD_EULER", "EULER_CROMER"};

    /**
     * @brief Updates @ref r "r" and @ref v "v" according to the Forward Euler method.
     * 
     * @details The Forward Euler method is given by
     * \f{align*}{
     *  v_{i+1} &= v_{i} + hf_{i}, \\ 
     *  r_{i+1} &= r_{i} + hv_{i},
     * \f}
     * where \f$ f_{i}\f$ is the discretized version of \f$ d^2r/dt^2 \f$.
     * 
     * @param particles Collection of particles.
     */
    void next_forwardEuler(std::vector<Particle> particles);

    /**
     * @brief Updates @ref r "r" and @ref v "v" according to the Euler-Cromer method.
     * 
     * @details The Euler-Cromer method is given by
     * \f{align*}{
     *  v_{i+1} &= v_{i} + hf_{i}, \\ 
     *  r_{i+1} &= r_{i} + hv_{i+1},
     * \f}
     * where \f$ f_{i}\f$ is the discretized version of \f$ d^2r/dt^2 \f$.
     * 
     * @param particles Collection of particles.
     */
    void next_EulerCromer(std::vector<Particle> particles);

    /**
     * @brief Updates the postion and momentum of the @ref particles "particles" using the Runge-Kutta 4 method. 
     * 
     * @details The Runge-Kutta 4 method is given by
     * \f{align}{
     *  k_{1,r} &= hv_{i}, \\ 
     *  k_{1,v} &= hf(t_i, r_i, v_i), \\ 
     *  k_{2,r} &= h(v_i + \frac{1}{2}k_{1,v}), \\ 
     *  k_{2,v} &= hf(t_i+\frac{1}{2}h, r_i + \frac{1}{2}k_{1,r}, v_i + \frac{1}{2}k_{1,v}), \\
     *  k_{3,r} &= h(v_i + \frac{1}{2}k_{2,v}), \\ 
     *  k_{3,v} &= hf(t_i+\frac{1}{2}h, r_i + \frac{1}{2}k_{2,r}, v_i + \frac{1}{2}k_{2,v}),
     *  k_{4,r} &= h(v_i + \frac{1}{2}k_{3,v}), \\ 
     *  k_{4,v} &= hf(t_i+\frac{1}{2}h, r_i + \frac{1}{2}k_{3,r}, v_i k_{3,v}),
     * \f}
     * and 
     * \f{align}{   
     *  r_{i+1} &= r_i + \frac{1}{6}(k_{1,r} + 2k_{2,r} + 2k_{3,r} + k_{4,r}), \\ 
     *  v_{i+1} &= v_i + \frac{1}{6}(k_{1,v} + 2k_{2,v} + 2k_{3,v} + k_{4,v}), \\ 
     * \f}
     * where  is the discretized version of \f$ f(t,x,v) = d^2r/dt^2 \f$. However, since the particles are dependent on the motion of the 
     * other particles in the Penning trap, the postion and velocities of the particles needs to be updated after each computation of \f$k_i\f$. 
     * Computing these \f$k_{i}\f$'s is done in @ref k_1_RK4 "k_1_RK4", @ref k_2_RK4 "k_2_RK4", @ref k_3_RK4 "k_3_RK4" and @ref k_4_RK4 "k_4_RK4".
     * 
     * @param particles Collection of particles.
     */
    void next_RK4(std::vector<Particle> particles);

    /**
     * @brief Computes \f$k_1\f$ in the RK4 method, see @ref next_RK4 "next_RK4".
     * 
     * @param r Position vector.
     * @param v Velocity vector.
     * @param h Step size.
     */
    void k_1_RK4(arma::vec &r, arma::vec &v, float h);
    
    /**
     * @brief Computes \f$k_2\f$ in the RK4 method, see @ref next_RK4 "next_RK4".
     * 
     * @param r Position vector.
     * @param v Velocity vector.
     * @param h Step size.
     */
    void k_2_RK4(arma::vec &r, arma::vec &v, float h);

    /**
     * @brief Computes \f$k_2\f$ in the RK4 method, see @ref next_RK4 "next_RK4".
     * 
     * @param r Position vector.
     * @param v Velocity vector.
     * @param h Step size.
     */
    void k_3_RK4(arma::vec &r, arma::vec &v, float h);

    /**
     * @brief Computes \f$k_2\f$ in the RK4 method, see @ref next_RK4 "next_RK4".
     * 
     * @param r Position vector.
     * @param v Velocity vector.
     * @param h Step size.
     */
    void k_4_RK4(arma::vec &r, arma::vec &v, float h);

public:

    /**
     * @brief Construct a new ODESolver object.
     * 
     * @param penningTrap Instance of the PenningTrap class.
     * @param solver The type of ODESolver to evolve the system.
     */
    SolverPenningTrap(PenningTrap &penningTrap, std::string solver="RK4");

    /**
     * @brief Evolves the system using the set solver method.
     * 
     * @param T The total time to evolve the system.
     * @param N The number of time points.
     */
    void evolve_system(const float T, const int N);

};