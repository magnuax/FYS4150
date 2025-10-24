import h5py
import numpy as np
from plot_trajectories import plot_trajectories
import matplotlib.pyplot as plt
plt.style.use('ggplot')


def plot_trapped(filename, amplitudes=[0.1,0.4,0.7], frequency_range=None, linestyle=None, colors=None, alpha=1.0):

    if not linestyle:
        linestyle = '-'

    if not colors:
        colors = [f'C{i}' for i in range(len(amplitudes))]

    if "$(FWILD)" not in filename:
        with h5py.File(filename, 'r') as dataset:
            data = np.array(dataset['escape_data'])
        
        frequencies = data[:, 0]
        fraction_inside = 1 - data[:, 1] / 100  # since we
        plt.plot(frequencies, fraction_inside, label=f'f={f}')

    else:
        for i, f in enumerate(amplitudes):
            filename_temp = filename.replace('$(FWILD)', f"{f:.1f}".replace('.', ''))

            with h5py.File(filename_temp, 'r') as dataset:
                data = np.array(dataset['escape_data'])

            frequencies = data[:, 0]
            fraction_inside = 1 - data[:, 1] / 100  # since we simulated 100 particles
            
            plt.plot(frequencies, fraction_inside, label=f'f={f}', linestyle=linestyle, color=colors[i], alpha=alpha)

    if frequency_range:
        plt.xlim(frequency_range)
    else:
        plt.xlim(frequencies.min(), frequencies.max())
    
    plt.xlabel(r'$\omega_V\ \ [MHz]$')
    plt.ylabel('Fraction of particles inside the trap')


if __name__ == "__main__":
    import os

    OUTDIR = '../figs/results/'
    if not os.path.exists(OUTDIR):
        os.makedirs(OUTDIR)

    B0 = 9.65*1e1
    V0 = 2.41*1e6
    d = 500
    m = 40
    q = 1 
    
    omega_0 = q * B0 / m
    omega_z = np.sqrt(2 * q * V0 / (m * d**2))

    temp = np.sqrt(omega_0**2 - 2 * omega_z**2)
    omega_1 = omega_z
    omega_2 = omega_z*2
    omega_3 = omega_z*3


    fig = plt.figure(figsize=(6,4))
    plot_trapped('out/n_escaped-p100-f$(FWILD)-RK4-int-n4000.h5')
    ylim = fig.gca().get_ylim()
    plt.ylim(ylim)
    plt.vlines([omega_1, omega_2, omega_3], ymin=ylim[0], ymax=ylim[1], colors=['k','k','k'], linestyles='dotted', zorder=1)
    plt.text(omega_1, ylim[1]*1.05, r'$\omega_\mathrm{z}$', ha='center', va='top', rotation=0)
    plt.text(omega_2, ylim[1]*1.05, r'$2\omega_\mathrm{z}$', ha='center', va='top', rotation=0)
    plt.text(omega_3, ylim[1]*1.05, r'$3\omega_\mathrm{z}$', ha='center', va='top', rotation=0)
    plt.legend()
    plt.savefig(OUTDIR + 'trapped_fraction.pdf')


    omega_range = (1, 1.7)
    fig = plt.figure(figsize=(6,4))
    linestyles = ['-', '--']
    types = ['int', 'nonint']
    alphas = [1.0, 0.7]
    for i in range(2):
        #plot_trapped(f'out/n_escaped-p100-f$(FWILD)-RK4-{types[i]}-n4000.h5', frequency_range=(1, 1.7), linestyle=linestyles[i], colors=['C0', 'C1', 'C2'], alpha=alphas[i])
        plot_trapped(f'out/n_escaped-p100-f$(FWILD)-RK4-{types[i]}-n4000.h5', frequency_range=omega_range, linestyle=linestyles[i], colors=['C0', 'C1', 'C2'], alpha=alphas[i])

    ylim = fig.gca().get_ylim()
    plt.ylim(ylim)
    plt.vlines([omega_2], ymin=ylim[0], ymax=ylim[1], colors=['k'], linestyles='dotted', zorder=1)
    plt.text(omega_2, ylim[1]*1.05, r'$2\omega_\mathrm{z}$', ha='center', va='top', rotation=0)
    plt.legend(['','','','f=0.1', 'f=0.4', 'f=0.7'],ncol=2, columnspacing=-0.3, loc='lower left')
    plt.savefig(OUTDIR + 'trapped_fraction_zoomed.pdf')

    plt.show()