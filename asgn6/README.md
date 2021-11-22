# Assignment 6: Public Key Cryptography
This set of programs implements RSA encryption using asymmetric keys. The three programs are `keygen` to generate public and private keys, 	`encrypt` to encrypt a file, and `decrypt` to decrypt an encrypted file.
# How to Build and Run
1. Clone this repository

2. Run `make` or `make all` in the `/asgn6` directory
	- Run `make keygen` to build only the key generator
	- Run `make encrypt` to build only the RSA encryption program
	- Run `make decrypt` to build only the RSA decryption program

3. Run `./keygen [-vhb:c:n:d:s:]` to run the key generator.
	- `-v` Print username & all key values
	- `-h` Print help message
	- `-b [value]` Set minimum bits for public key (default: 256)
	- `-c [iters]` Set number of iterations for Miller-Rabin prime testing (default: 50)
	- `-n [file]` Set public key file (default: `rsa.pub`)
	- `-d [file]` Set private key file (default: `rsa.priv`)
	- `-s [seed]` Set seed for random state (default: `time(NULL)`)
4. Run `./encrypt [-vhi:o:n:]` to run the encryptor, and `./decrypt [-vhi:o:n:]` to run the decryptor.
- Options:
	- `-v` Prints the original file size, compressed file size, and percent of space saved.
	- `-h` Prints help message. 
	- `-i [file]` Specifies file from which program should read input (default: stdin).
	- `-o [file]` Specifies file from which program should write to (default: stdout).
	- `-n [file]` Specifies file where keys are (default: `rsa.pub` for encryptor, `rsa.priv` for decryptor)

5.  Run `make clean` to remove object files and executables after building.
# Makefile
**Compiler**: `clang`  
**Flags**: `-Wall -Wpedantic -Werror -Wextra $(pkg-config --cflags gmp)`  
**Clean**: `make clean` removes executables and object files  
**Format**: `make format` formats all `.c` and `.h` files