import numpy as np
q = 1     # Charge [e] (elementary charge)
m = 40    # Mass [u] (atomic mass unit)
d = 500   # Characteristic dimension [μm]

def analytical_solution(t, r_0, v_0, B0=9.65*1e1, V0=2.41*1e6):
    """
    Returns the analytical solution for the z-coordinate of a single particle in a Penning trap.
    where omega_z = sqrt(2 * q * V0 / (m * d^2)).
    """
    omega_0 = q * B0 / m
    omega_z = np.sqrt(2 * q * V0 / (m * d**2))

    x_0, y_0, z_0 = r_0
    v_x0, v_y0, v_z0 = v_0

    temp = np.sqrt(omega_0**2 - 2 * omega_z**2)
    omega_pos = 0.5 * (omega_0 + temp)
    omega_neg = 0.5 * (omega_0 - temp)

    A_pos =  np.sqrt((v_y0 + x_0*omega_neg)**2 + (v_x0 - y_0*omega_neg)**2) / (omega_pos - omega_neg)
    A_neg =  np.sqrt((v_y0 + x_0*omega_pos)**2 + (v_x0 - y_0*omega_pos)**2) / (omega_pos - omega_neg)
    
    phi_plus = np.arctan((v_x0 - y_0*omega_neg)/(x_0*omega_neg+v_y0))
    if (x_0*omega_pos + v_y0) >= 0:
        phi_plus += np.pi 
    
    phi_minus = np.arctan((v_x0 - y_0*omega_pos)/(x_0*omega_pos+v_y0)) + np.pi
    if (x_0*omega_pos + v_y0) >= 0:
        phi_minus -= np.pi 


    f = A_pos*np.exp(-1j*(omega_pos*t + phi_plus)) + A_neg*np.exp(-1j*(omega_neg*t + phi_minus))
    x = np.real(f)
    y = np.imag(f)
    z = z_0 * np.cos(omega_z * t)

    return x, y, z

def rel_error(dataset, r_0, v_0):
    """
    Computes the relative error between the numerical and analytical solutions.
    """
    t_numerical = dataset['times'][0,:]
    r_numerical = dataset['positions'][:,:,0].T # e.g. (4000, 3)

    x_analytical, y_analytical, z_analytical = analytical_solution(t_numerical, r_0, v_0)
    
    r_analytical = np.array([x_analytical, y_analytical, z_analytical]).T  # e.g. (4000, 3)
    return np.linalg.norm(r_numerical - r_analytical, axis=1) / np.linalg.norm(r_analytical, axis=1)


if __name__ == "__main__":
    import h5py
    import matplotlib.pyplot as plt
    import os
    plt.style.use('ggplot')

    OUTDIR = '../figs/results/'
    if not os.path.exists(OUTDIR):
        os.makedirs(OUTDIR)
 
    r_0 = np.array([20, 0, 20])  # Initial position [um]
    v_0 = np.array([0, 25, 0])    # Initial velocity

    for method in ['FE', 'RK4']:
        plt.figure(figsize=(6,4))
        for n_steps in [4000, 8000, 16000, 32000]:
            # print(f'out/p1-{method}-int-n{n_steps}.h5')
            with h5py.File(f'out/p1-{method}-int-n{n_steps}.h5', 'r') as dataset:
                error = rel_error(dataset, r_0, v_0)
                plt.plot(dataset['times'][0,:], error, label=f'n_steps={n_steps}')

        plt.xlabel(r'$t\ \ [\mu s]$')
        plt.ylabel(r'Relative error in $z$')
        plt.yscale('log')
        plt.legend()
        plt.savefig(OUTDIR + f'relerror-{method}.pdf')
