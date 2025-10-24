import numpy as np
import matplotlib.pyplot as plt
import sys
plt.style.use("ggplot")

DATA_PATH = "build/data"
FIG_PATH = "build/figs"

plot_numerical = False
figname = "exact_solution.pdf"

# Plot exact solution:
data_exact = np.loadtxt(f"{DATA_PATH}/exact_solution.csv", delimiter=",")
x_exact = data_exact[:,0]
u_exact = data_exact[:,1]

plt.figure(figsize=(8,4))
plt.plot(x_exact, u_exact, "r--", label="exact solution", zorder=10)


# Plot numerical solution if true:
if plot_numerical:
    figname = "num_solution.pdf"
    
    # Plot numerical solutions from the Thomas algorithm:
    powers = np.array([1, 2, 3, 4])
    for p in powers:
        #data_num = np.loadtxt(f"data/special_n-1e{p}.csv", delimiter=",")
        data_num = np.loadtxt(f"{DATA_PATH}/thomas_n-1e{p}.csv", delimiter=",")
        
        n_steps = int(10**p)
        x_num = data_num[:,0]
        v_num = data_num[:,1]
        h = (x_num[-1] - x_num[1])/n_steps
        
        plt.plot(x_num, v_num, label=f"n = {n_steps}")
        
        
plt.xlabel("x")
plt.ylabel("u(x)")
plt.xlim(0, 1)
plt.ylim(0, 1)
plt.legend()
plt.savefig(f"{FIG_PATH}/{figname}")
