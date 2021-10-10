
# Assignment 2 - A Little Slice of Pi

  

This program calculates mathematical constants based on different converging series. 

  

# How to Build and Run

  

1. Clone this repository

2. Run `make` in the `/asgn2` directory

3. Run `./mathlib-test [-aebmrvnsh]` to run the program
- Options:
	- `-a` Runs all tests.
	- `-e` Runs e test.
	- `-b` Runs bbp pi test. 
	- `-m` Runs Madhava pi test.
	- `-r` Runs Euler pi test.
	- `-v` Runs Viete pi test.
	- `-n` Runs Newton square root tests. 
	- `-s` Prints the number of terms calculated in each test.
	- `-h` Prints the help message. 

4.  Run `make clean` to remove object files and executables after building
# Makefile
**Compiler**: `clang` 
**Flags**: `-Wall -Wpedantic -Werror -Wextra`
**Clean**: `make clean` removes executables and object files
**Format**: `make format` formats all `.c` and `.h` files