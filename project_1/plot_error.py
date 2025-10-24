import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
from tabulate import tabulate
plt.style.use("ggplot")

DATA_PATH = "build/data"
FIG_PATH = "build/figs"

# Plot exact solution:
data_exact = np.loadtxt(f"{DATA_PATH}/exact_solution.csv", delimiter=",")

x_exact = data_exact[:,0]
u_exact = data_exact[:,1]

interp_exact = interp1d(x_exact, u_exact, bounds_error=False, fill_value="extrapolate")

N = len(x_exact)

fig_abs, ax_abs = plt.subplots(figsize=(8,4))
fig_rel, ax_rel = plt.subplots(figsize=(8,4))

# Plot numerical solutions from the Thomas algorithm:
for n_steps in [10, 100, 1000, 10000]:
    data_num = np.loadtxt(f"{DATA_PATH}/thomas_n-1e{int(np.log10(n_steps))}.csv", delimiter=",")
    x_num = data_num[1:-1,0]
    v_num = data_num[1:-1,1]
    u_anal = interp_exact(x_num)    
    
    abs_err = np.abs(v_num - u_anal)
    rel_err = np.abs((v_num - u_anal)/u_anal)

    ax_abs.plot(x_num, np.log10(abs_err), label=f"n = {n_steps}")
    ax_rel.plot(x_num, np.log10(rel_err), label=f"n = {n_steps}")


ax_abs.set_xlabel("x")
ax_abs.set_ylabel(r"$\mathrm{log}_{10}\Delta_i$")
ax_abs.set_xlim(0, 1)
ax_abs.legend()
fig_abs.savefig(f"{FIG_PATH}/abs_err.pdf")

ax_rel.set_xlabel("x")
ax_rel.set_ylabel(r"$\mathrm{log}_{10}\varepsilon_i$")
ax_rel.set_xlim(0, 1)
ax_rel.legend()
fig_rel.savefig(f"{FIG_PATH}/rel_err.pdf")


# PLOT MAX REL ERR
plt.figure(figsize=(8,4))

max_err = []
n_array = np.array([1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7])
powers = np.array([1, 2, 3, 4, 5, 6, 7])

for p in powers:
    data_num = np.loadtxt(f"{DATA_PATH}/thomas_n-1e{p}.csv", delimiter=",")

    x_num = data_num[1:-1,0]
    v_num = data_num[1:-1,1]
    u_anal = interp_exact(x_num)
    
    abs_err = np.abs(v_num - u_anal)
    rel_err = np.abs((v_num- u_anal)/u_anal)
    
    abs_err[0]  = 0
    abs_err[-1] = 0
    rel_err[0]  = 0
    rel_err[-1] = 0
    
    max_err.append( np.max(rel_err))


max_err = np.array(max_err)    

table = np.transpose([powers,max_err])
table = tabulate(table, headers=["log₁₀(n_steps)", "max(ε)"], tablefmt="latex")
print(table)

plt.plot(powers, max_err)

plt.xlabel("$\log(n_\mathrm{steps})$")
plt.ylabel(r"$\mathrm{max}(\varepsilon)$")
plt.savefig(f"{FIG_PATH}/max_error.pdf")

