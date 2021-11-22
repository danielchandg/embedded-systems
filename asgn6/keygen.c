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

#define OPTIONS "b:i:n:d:s:vh"

// Helper function to print help message.
void help_string(char *cwd) {
    fprintf(stdout,
        "SYNOPSIS\n   Generates an RSA public/private key pair.\n\nUSAGE\n   %s [-hv] [b bits] -n "
        "pbfile -d pvfile\n\nOPTIONS\n   -h              Display program help and usage.\n   -v    "
        "          Display verbose program output.\n   -b bits         Minimum bits needed for "
        "public "
        "key n.\n   -c confidence   Miller-Rabin iterations for testing primes (default: 50).\n   "
        "-n pbfile       Public key file (default: rsa.pub).\n   -d pvfile       Private key file "
        "(default: rsa.priv).\n   -s seed         Random seed for testing.\n",
        cwd);
}
int main(int argc, char **argv) {
    int64_t opt = 0;
    uint64_t bits = 256, iters = 50, seed = time(NULL);
    bool verbose = false;
    FILE *pbfile = fopen("rsa.pub", "w+"), *pvfile = fopen("rsa.priv", "w+");
    char *cwd = argv[0];

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b':
            bits = strtoul(optarg, NULL, 10);
            if (errno == EINVAL) {
                bits = 256;
            }
            break;
        case 'i':
            iters = strtoul(optarg, NULL, 10);
            if (errno == EINVAL) {
                iters = 50;
            }
            break;
        case 'n': pbfile = fopen(optarg, "w+"); break;
        case 'd': pvfile = fopen(optarg, "w+"); break;
        case 's':
            seed = strtoul(optarg, NULL, 10);
            if (errno == EINVAL) {
                seed = time(NULL);
            }
            break;
        case 'v': verbose = true; break;
        case 'h': help_string(cwd); return 0;
        case '?': help_string(cwd); return 0;
        }
    }
    // Update file permissions and initialize random state.
    fchmod(fileno(pvfile), 0600);
    randstate_init(seed);
    srandom(seed);

    mpz_t p, q, n, e, d, user, s;
    mpz_inits(p, q, n, e, d, user, s, NULL);
    // Make public and private keys.
    rsa_make_pub(p, q, n, e, bits, iters);
    rsa_make_priv(d, e, p, q);
    char *username = getenv("USER");
    mpz_set_str(user, username, 62);
    rsa_sign(s, user, d, n);
    // Write keys to file.
    rsa_write_pub(n, e, s, username, pbfile);
    rsa_write_priv(n, d, pvfile);
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
        mpz_set(counter, p);
        bit_count = 0;
        while (mpz_cmp_ui(counter, 0) > 0) {
            bit_count++;
            mpz_fdiv_q_ui(counter, counter, 2);
        }
        gmp_printf("p (%d bits) = %Zd\n", bit_count, p);
        mpz_set(counter, q);
        bit_count = 0;
        while (mpz_cmp_ui(counter, 0) > 0) {
            bit_count++;
            mpz_fdiv_q_ui(counter, counter, 2);
        }
        gmp_printf("q (%d bits) = %Zd\n", bit_count, q);
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
        mpz_set(counter, d);
        bit_count = 0;
        while (mpz_cmp_ui(counter, 0) > 0) {
            bit_count++;
            mpz_fdiv_q_ui(counter, counter, 2);
        }
        gmp_printf("d (%d bits) = %Zd\n", bit_count, d);
        mpz_clear(counter);
    }
    fclose(pbfile);
    fclose(pvfile);
    mpz_clears(p, q, n, e, d, user, s, NULL);
    randstate_clear();
}
