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

void help_string(char *cwd) {
    fprintf(stdout,
        "SYNOPSIS\n   Encrypts data using RSA encryption.\n   Encrypted data is decrypted by the "
        "decrypt program.\n\nUSAGE\n   %s [-hv] [-i infile] [-o outfile] -n pubkey\n\nOPTIONS\n   "
        "-h              Display program help and usage.\n   -v              Display verbose "
        "program output.\n   -i infile       Input file of data to encrypt (default: stdin).\n   "
        "-o outfile      Output file for encrypted data (default: stdout).\n   -n pbfile       "
        "Public key file (default: rsa.pub).\n",
        cwd);
}
int main(int argc, char **argv) {
    int64_t opt = 0;
    bool verbose = false;
    FILE *infile = stdin, *outfile = stdout, *pbfile = fopen("rsa.pub", "r");
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
            pbfile = fopen(optarg, "r");
            if (pbfile == NULL) {
                fprintf(stderr, "Error: failed to open pbfile.\n");
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
    if (pbfile == NULL) {
        fprintf(stderr, "Error: failed to open pbfile.\n");
        return 0;
    }
    mpz_t n, e, s, user;
    mpz_inits(n, e, s, user, NULL);
    char *username = (char *) calloc(256, sizeof(char));
    rsa_read_pub(n, e, s, username, pbfile);
    int bit_count;
    if (verbose) {
        printf("user = %s\n", username);
        mpz_t counter;
        mpz_init_set(counter, s);
        bit_count = 0;
        while (mpz_cmp_ui(counter, 0) > 0) {
            bit_count++;
            mpz_fdiv_q_ui(counter, counter, 2);
        }
        gmp_printf("s (%d bits) = %Zd\n", bit_count, s);
        mpz_set(counter, n);
        bit_count = 0;
        while (mpz_cmp_ui(counter, 0) > 0) {
            bit_count++;
            mpz_fdiv_q_ui(counter, counter, 2);
        }
        gmp_printf("n (%d bits) = %Zd\n", bit_count, n);
        mpz_set(counter, e);
        bit_count = 0;
        while (mpz_cmp_ui(counter, 0) > 0) {
            bit_count++;
            mpz_fdiv_q_ui(counter, counter, 2);
        }
        gmp_printf("e (%d bits) = %Zd\n", bit_count, e);
        mpz_clear(counter);
    }
    mpz_set_str(user, username, 62);
    if (!rsa_verify(user, s, e, n)) {
        fprintf(stderr, "Error: failed to verify signature.\n");
        return 0;
    }
    rsa_encrypt_file(infile, outfile, n, e);
    fclose(infile);
    fclose(pbfile);
    fclose(outfile);
    free(username);
    username = NULL;
    mpz_clears(n, e, s, user, NULL);
}
