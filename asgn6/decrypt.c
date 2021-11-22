#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <errno.h>
#include <gmp.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "randstate.h"
#include "rsa.h"
#include "numtheory.h"

#define OPTIONS "i:o:n:vh"

// Helper function if help message is requested
void help_string(char *cwd) {
    fprintf(stdout,
        "SYNOPSIS\n   Decrypts data using RSA decryption.\n   Encrypted data is encrypted by the "
        "encrypt program.\n\nUSAGE\n   %s [-hv] [-i infile] [-o outfile] -d privkey\n\nOPTIONS\n   "
        "-h              Display program help and usage.\n   -v              Display verbose "
        "program output.\n   -i infile       Input file of data to decrypt (default: stdin).\n   "
        "-o outfile      Output file for decrypted data (default: stdout).\n   -n pvfile       "
        "Private key file (default: rsa.priv).\n",
        cwd);
}
int main(int argc, char **argv) {
    int64_t opt = 0;
    bool verbose = false;
    FILE *infile = stdin, *outfile = stdout, *pvfile = fopen("rsa.priv", "r");
    char *cwd = argv[0];
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            infile = fopen(optarg, "r");
            if (infile == NULL) {
                fprintf(stderr, "Error: failed to open infile.\n");
                return 0;
            }
            break;
        case 'n':
            pvfile = fopen(optarg, "r");
            if (pvfile == NULL) {
                fprintf(stderr, "Error: failed to open pvfile.\n");
                return 0;
            }
            break;
        case 'o':
            outfile = fopen(optarg, "w");
            if (outfile == NULL) {
                fprintf(stderr, "Error: failed to open outfile.\n");
                return 0;
            }
            break;
        case 'v': verbose = true; break;
        case 'h': help_string(cwd); return 0;
        case '?': help_string(cwd); return 0;
        }
    }
    if (pvfile == NULL) {
        fprintf(stderr, "Error: failed to open pvfile.\n");
        return 0;
    }
    mpz_t n, d;
    mpz_inits(n, d, NULL);
    // Read private keys
    rsa_read_priv(n, d, pvfile);
    int bit_count;
    if (verbose) {
        mpz_t counter;
        mpz_init_set(counter, n);
        bit_count = 0;
        while (mpz_cmp_ui(counter, 0) > 0) {
            bit_count++;
            mpz_fdiv_q_ui(counter, counter, 2);
        }
        gmp_printf("n (%d bits) = %Zd\n", bit_count, n);
        mpz_set(counter, d);
        bit_count = 0;
        while (mpz_cmp_ui(counter, 0) > 0) {
            bit_count++;
            mpz_fdiv_q_ui(counter, counter, 2);
        }
        gmp_printf("d (%d bits) = %Zd\n", bit_count, d);
        mpz_clear(counter);
    }
    // Decrypt file using private key
    rsa_decrypt_file(infile, outfile, n, d);
    fclose(infile);
    fclose(pvfile);
    fclose(outfile);
    mpz_clears(n, d, NULL);
}
