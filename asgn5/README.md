# Assignment 5: Huffman Coding
This pair of program encodes and decodes a file using the Huffman Coding algorithm.
# How to Build and Run
1. Clone this repository

2. Run `make` or `make all` in the `/asgn5` directory
	- Run `make encode` to build only the Huffman encoder
	- Run `make decode` to build only the Huffman decoder

3. Run `./encode [-vhi:o:]` to run the encoder.
4. Run `./decode [-vhi:o:]` to run the decoder.
- Options:
	- `-v` Prints the original file size, compressed file size, and percent of space saved.
	- `-h` Prints help message. 
	- `-i <file>` Specifies file from which program should read input (default: stdin).
	- `-o <file>` Specifies file from which program should write to (default: stdout).

5.  Run `make clean` to remove object files and executables after building.
# Makefile
**Compiler**: `clang`  
**Flags**: `-Wall -Wpedantic -Werror -Wextra`  
**Clean**: `make clean` removes executables and object files  
**Format**: `make format` formats all `.c` and `.h` files