import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
plt.style.use("ggplot")

powers = np.array([2,3,4,5,6,7])

DATA_PATH = "build/data"
FIG_PATH = "build/figs"
#print(timings_gen[:10])

t_gen  = []
dt_gen = []
t_spc  = []
dt_spc  = []

#n_steps  = [f"1e{i}" for i in range(1,4+1)]
n_steps  = ["1e1"]
n_steps += [f"{i}e5" for i in range(1,9+1)]
n_steps += ["1e6"]
powers = np.log([float(n) for n in n_steps])
"""
for p in powers:
    timings_gen = pd.read_csv(f"{DATA_PATH}/timings_general_n-1e{p}.txt", sep=" ", header=None)[2]
    
    std_gen = timings_gen.std()
    avg_gen = timings_gen.mean()
    
    std_spc = timings_gen.std()
    avg_spc = timings_gen.std()
    
    t_gen.append(avg_gen)
    dt_gen.append(std_gen)
    t_spc.append(avg_spc)
    dt_spc.append(std_spc)
"""    
  
for n in n_steps:
    timings_gen = pd.read_csv(f"{DATA_PATH}/timings_general_n-{n}.txt", sep=" ", header=None)[2]
    timings_spc = pd.read_csv(f"{DATA_PATH}/timings_special_n-{n}.txt", sep=" ", header=None)[2]
    #timings_gen = timings_gen/timings_gen.max()
    #timings_spc = timings_spc/timings_gen.max()
    
    std_gen = timings_gen.std()
    avg_gen = timings_gen.mean()

    std_spc = timings_spc.std()
    avg_spc = timings_spc.mean()

    t_gen.append(avg_gen)
    dt_gen.append(std_gen)
    t_spc.append(avg_spc)
    dt_spc.append(std_spc)
    
t_gen = np.array(t_gen)
dt_gen = np.array(dt_gen)
t_spc = np.array(t_spc)
dt_spc = np.array(dt_spc)


n_steps = [float(n) for n in n_steps]
#powers = np.log10(n_steps)
powers = n_steps

plt.figure(figsize=(8,4))

# normalize runtime:
t_max = np.max(t_gen)
t_gen = t_gen/t_max
t_spc = t_spc/t_max
dt_gen = dt_gen/t_max
dt_spc = dt_spc/t_max


plt.errorbar(powers, t_gen, dt_gen, label="general", elinewidth=1, capsize=2)
plt.fill_between(powers, t_gen - dt_gen, t_gen + dt_gen, color='r', alpha=0.2)


plt.errorbar(powers, t_spc, dt_spc, label="special", elinewidth=1)
#plt.plot(powers, t_spc, label="special", marker="o")
plt.fill_between(powers, t_spc - dt_spc, t_spc + dt_spc, color='b', alpha=0.2)

plt.xlim(0,1e6)
plt.xlabel(r"$n_\mathrm{steps}$")
plt.ylabel("Runtime [a.u.]")
plt.legend()
#plt.savefig(f"{FIG_PATH}/times_normalized.pdf")
plt.show()
