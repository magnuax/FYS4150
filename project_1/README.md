# Project 1

## Description
See [FYS3150/project1](https://anderkve.github.io/FYS3150/book/projects/project1.html).

## Code structure
* `src/tridiagonal_algorithms.cpp` contains implementations of the special and general algorithms
* `src/utils.cpp` contains a file-writing function, as well as implementations of the source term and analytical solution to the Poisson equation

These are called on by the main programs

1) `exact_solution.cpp`
2) `thomas_algorithm.cpp`
3) `special_algorithm.cpp`

in order to provide a (approximate) solution the Poisson equation using either the known analytical solution, the Thomas algorithm, or the special algorithm respectively.

## Build Instructions

The executables are built using
   ```bash
   make all
   ```
Resulting executables for the main programs are then found in the `build` folder.

## Running the solvers

The solvers (i.e. `special_algoritm.cpp` or ``thomas_algoritm.cpp``) are used in the following way:

```bash
./<solver> <n_steps> <filename>
```
This will also run a single timing test and print the result in the terminal.
To run repeated tests, it is easiest to do a for loop in your terminal and direct the output to a file of your choice.
One can direct the output to a file in the following way:
```bash
./<solver> <n_steps> <filename_1> >> <filename_2>
```
