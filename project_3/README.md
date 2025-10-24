# Project 3: Computational Physics

## Description
See [FYS3150/project3](https://anderkve.github.io/FYS3150/book/projects/project3.html).
This repository contains the code and documentation for Project 3 in the FYS3150 - Computational Physics course at the University of Oslo.

## Structure

- **`src/`**: Contains all source code files for the project.
- **`include/`** Contains relevant header files for the project.
- **`out/`**: Save location for data from main program.

## Requirements
### Main program
To compile and run the main program, the most basic dependencies are:
- Armadillo (C++ linear algebra library)
- HDF5 (Hierarchical Data Format library)

Additionally, compiling using the `makefile` requires 
- g++
- CMake


On Debian-based systems, for instance, all of the above can be installed by running
```bash
sudo apt-get install g++ cmake libarmadillo-dev libhdf5-dev
```
in your terminal.

### Plotting scrips
To run the plotting scripts, you need the following dependencies:

- Python 3.x
- NumPy
- Matplotlib
- h5py

These can be installed by running

```bash
pip install -r requirements.txt
```
in the root folder.

## Usage
### Compiling and running entire project using make
The entire project may be compiled and run using
```bash
make project 
```
This includes generating data for all parts of problem 8 and 9, as well as generating plots using the python scripts.

Output from problem 9, which would normally print to the terminal, will in this case be directed to a logfile `problem9.log`.

### Compiling & running the main program manually
1. Compile:
    ```bash
    make all
    ```

2. The main program can also be run manually, using the following optional arguments: 
    ```
    Usage:
        ./build/main [options]

    Options:
        --problem_8                 Run problem 8
        --problem_9                 Run problem 9
        --n_steps <int>             Number of time steps (default: 4000)
        --T <double>                Total time (default: 50.0)
        --n_particles <int>         Number of particles (default: 2)
        --non-interacting           Disable particle interactions
        --seed <int>                Random seed (default: 1234)
        --help                      Show this help message

    Options for problem 9:
        --amplitude <double>        Amplitude of the oscillating field (default: 0.1)
        --freq_min <double>         Minimum frequency (default: 0.2)
        --freq_max <double>         Maximum frequency (default: 2.5)
        --n_freq <int>              Number of frequency values (default: 10)

    ```
    All data will be stored as an HDF5 (`.h5`) file, in the **`out/`** folder.
    Data files are in general named (and therefore distinguished) based on 
    - Number of time steps
    - Number of particles
    - Whether interactions are turned on/off
    - Amplitude of time dependent part of potential (Problem 9 only)

    Note especially that in problem 9, running the program will by attempt to update existing data if available.
    Therefore, existing points in the dataset will **NOT** be recomputed or overriden.

### Plotting scripts
A few plotting scripts are also included:

**Problem 8**
- `plot_trajectories.py` - Generates various plots of both position space & phase space trajectories of particles in the Penning trap.
- `plot_errors.py` - Generates plots of the relative error of RK4/FE methods for a single particle.

**Problem 9**
- `plot_trapped.py` - Generates plots of the fraction of trapped particles remaining in the penning trap.

These can be run as any other python program, and all figures will be saved to the **`plots/`** folder.
Note that some tweaks may be necessary if using different datasets/parameters than the ones used by us.