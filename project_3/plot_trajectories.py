import h5py
import numpy as np
import matplotlib.pyplot as plt
plt.style.use('ggplot')

def plot_trajectories(**kwargs):
    """
    Plots the trajectories of particles given their x and y coordinates.
    Possible keyword arguments:
    
    **kwargs:
        [....] (array): A 2D numpy array with shape (N, 2) representing coordinates of particle i. Key-name [....] will be used as the legend label, replacing underscores with spaces.
        xlabel (string): Label for the x-axis.
        ylabel (string): Label for the y-axis.
    """
        
    if 'xlabel' in kwargs:
        plt.xlabel(kwargs.pop('xlabel'))

    if 'ylabel' in kwargs:
        plt.ylabel(kwargs.pop('ylabel'))

    i = 0
    for key, data in kwargs.items():
    
        x = data[:,0]
        y = data[:,1]

        label = key.replace('_', ' ')
        plt.scatter(x[0], y[0], color=f"C{i}", zorder=10)  # Mark the start point
        plt.scatter(x[0], y[0], color=f"w", s=50, linewidths=1.5, zorder=9)  # Mark the end point
        plt.scatter(x[-1], y[-1], marker="x", color=f"C{i}", zorder=10)  # Mark the end point
        plt.scatter(x[-1], y[-1], marker="x", color=f"w", s=50, linewidths=2.5, zorder=9)  # Mark the end point
        i += 1
        plt.plot(x, y, label=label)

    plt.legend()
    
if __name__ == "__main__":
    import os

    OUTDIR = '../figs/results/'
    if not os.path.exists(OUTDIR):
        os.makedirs(OUTDIR)
        
    ##########################
    # Single particle tz plot
    ##########################

    dataset = h5py.File('out/p1-RK4-int-n4000.h5', 'r')

    tz = np.array([dataset['times'][0,:], dataset['positions'][2,:,0]]).T
    plt.figure(figsize=(6,4))
    plot_trajectories(Particle_1 = tz,
                    xlabel = r'$t\ \ [\mu s]$',
                    ylabel = r'$z\ \ [\mu m]$')
    plt.savefig(OUTDIR + 'single-tz-RK4-n4000.pdf')

    for n_steps in [4000]:
        for type in ['int', 'nonint']:
            with h5py.File(f'out/p2-RK4-{type}-n{n_steps}.h5', 'r') as dataset:
                # Positions and velocities of particle 1 and 2 respectively
                r1 = dataset['positions'][:,:,0].T
                v1 = dataset['velocities'][:,:,0].T
                r2 = dataset['positions'][:,:,1].T
                v2 = dataset['velocities'][:,:,1].T
                
            #############################
            # Plot trajectories xy-plane
            ##############################
            
            plt.figure(figsize=(6,6))
            plot_trajectories(Particle_1 = r1[:, 0:2],
                                Particle_2 = r2[:, 0:2],
                                xlabel = r'$x\ \ [\mu m]$',
                                ylabel = r'$y\ \ [\mu m]$')
            plt.gca().set_aspect('equal', adjustable='box')
            plt.savefig(OUTDIR + f'{type}-xy-RK4-n{n_steps}.pdf')

            #####################################
            # Plot x & z phase space trajectories
            #####################################
            for idx, label in [(0, 'x'), (2, 'z')]:
                temp_1 = np.array([r1[:,idx], v1[:, idx]]).T
                temp_2 = np.array([r2[:,idx], v2[:, idx]]).T

                plt.figure(figsize=(6,6))
                plot_trajectories(Particle_1 = temp_1, 
                                    Particle_2 = temp_2,
                                    xlabel = r'$' +      label   + r'\ \ [\mu m/\mu s]$',
                                    ylabel = r'$' + f'v_{label}' + r'\ \ [\mu m/\mu s]$')
                plt.savefig(OUTDIR + f'phasespace-{label}-{type}-RK4-n{n_steps}.pdf')
            