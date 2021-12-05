# Assignment 7: The Great Firewall of Santa Cruz
This program holds a memory of "badspeak" and "oldspeak" words, and parses `stdin` using regex to detect if words belong to badspeak, oldspeak, or neither.
# How to Build and Run
1. Clone this repository

2. Run `make` or `make all` in the `/asgn7` directory

3. Run `./banhammer [-ht:f:s]` to run the key generator.
	- `-h` Print help message
	- `-t [size]` Hash Table size (default: 2^16)
	- `-f [size]` Bloom Filter size(default: 2^20)
	- `-s` Enable statistics printing

4.  Run `make clean` to remove object files and executables after building.
# Makefile
**Compiler**: `clang`  
**Flags**: `-Wall -Wpedantic -Werror -Wextra  
**Clean**: `make clean` removes executables and object files  
**Format**: `make format` formats all `.c` and `.h` files