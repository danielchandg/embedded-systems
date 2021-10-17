# Assignment 3 - Sorting: Putting your affairs in order

  

This program sorts an array using 4 sorting algorithms: Quick Sort, Heap Sort, Shell Sort, and Insertion Sort.

  

# How to Build and Run

  

1. Clone this repository

2. Run `make` in the `/asgn3` directory

3. Run `./sorting [-haeisqn:p:r:]` to run the program
- Options:
	- `-a` Runs all four sorting algorithms.
	- `-e` Runs Heap Sort.
	- `-i` Runs Insertion Sort. 
	- `-s` Runs Shell Sort.
	- `-q` Runs Quick Sort.
	- `-n <length>` Specifies the number of array elements (default: 100).
	- `-p <elements>` Specifies number of array elements to print (default: 100).
	- `-r <seed>` Initializes random seed (default: 13371453).
	- `-h` Prints the help message. 

4.  Run `make clean` to remove object files and executables after building
# Makefile
**Compiler**: `clang` 
**Flags**: `-Wall -Wpedantic -Werror -Wextra`
**Clean**: `make clean` removes executables and object files
**Format**: `make format` formats all `.c` and `.h` files