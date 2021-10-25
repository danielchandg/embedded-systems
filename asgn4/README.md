# Assignment 4 - # Perambulations of Denver Long

  

This program finds the shortest Hamiltonian path of a graph.

  

# How to Build and Run

  

1. Clone this repository

2. Run `make` in the `/asgn4` directory

3. Run `./sorting [-uvhi:o:]` to run the program
- Options:
	- `-u` Specifies the graph to be undirected.
	- `-v` Prints Hamiltonian paths as they are found.
	- `-h` Prints help message. 
	- `-i <file>` Specifies file from which program should read graph (default: stdin).
	- `-o <file>` Specifies file from which program should write to (default: stdout).

4. The input file should be in the following format:
	- First line is one integer *n*. This is the number of cities.
	- The next *n* lines should be strings. These are the city names.
	- The remaining lines until the end of the file should be 3 integers, which are `city1`, `city2`, and `weight`. This is an edge from `city1` to `city2` with weight `weight`.

5.  Run `make clean` to remove object files and executables after building.
# Makefile
**Compiler**: `clang`  
**Flags**: `-Wall -Wpedantic -Werror -Wextra`  
**Clean**: `make clean` removes executables and object files  
**Format**: `make format` formats all `.c` and `.h` files  