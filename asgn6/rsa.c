#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <unistd.h>
#include <fcntl.h>

#include "rsa.h"
#include "randstate.h"
#include "numtheory.h"

// Make RSA public keys.
// Output: (prime1, prime2, prime1*prime2, exponent)
void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    uint64_t p_bits = (random() % (nbits / 2)) + ((nbits + 3) / 4);
    uint64_t q_bits = nbits - p_bits + 2;
    make_prime(p, p_bits, iters);
    make_prime(q, q_bits, iters);
    mpz_mul(n, p, q);
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_t totient, base, gcd_val;
    mpz_init(totient);
    mpz_mul(totient, p, q);
    mpz_add_ui(p, p, 1);
    mpz_add_ui(q, q, 1);
    mpz_init_set_ui(base, 1);
    for (uint64_t i = 0; i < nbits - 1; i++) {
        mpz_mul_ui(base, base, 2);
    }
    mpz_init(gcd_val);
    do {
        mpz_urandomb(e, state, nbits - 1);
        mpz_add(e, e, base);
        gcd(gcd_val, e, totient);
    } while (mpz_get_ui(gcd_val) != 1);
    mpz_clears(totient, base, gcd_val, NULL);
}
// Write public keys to pbfile.
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    fprintf(pbfile, "%s\n", username);
}
// Read public keys from pbfile.
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%Zx\n", e);
    gmp_fscanf(pbfile, "%Zx\n", s);
    fscanf(pbfile, "%s\n", username);
}
// Make private keys.
// Input: e, p, q.
// Output: d
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    mpz_t totient;
    mpz_init(totient);
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(totient, p, q);
    mpz_add_ui(p, p, 1);
    mpz_add_ui(q, q, 1);
    mod_inverse(d, e, totient);
    mpz_clear(totient);
}
// Write private keys to pvfile.
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
}
// Read private keys from pvfile.
void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", n);
    gmp_fscanf(pvfile, "%Zx\n", d);
}
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n);
}
// Encrypt data in infile using n and e.
// Write encrypted data to outfile.
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    uint64_t k = 0;
    int fd = fileno(infile);
    mpz_t ntemp, m; // ntemp is used for counting bits in n and encrypted m.
    mpz_init_set(ntemp, n);
    mpz_init(m);
    while (mpz_get_ui(ntemp) > 0) {
        k++;
        mpz_fdiv_q_ui(ntemp, ntemp, 2);
    }
    k = (k - 1) / 8;
    uint8_t *block = (uint8_t *) calloc(k, sizeof(uint8_t));
    block[0] = 0xFF;
    int j = read(fd, &(block[1]), k - 1);
    while (j > 0) {
        mpz_import(m, j + 1, 1, 1, 1, 0, block);
        rsa_encrypt(ntemp, m, e, n);
        gmp_fprintf(outfile, "%Zx\n", ntemp);
        j = read(fd, &(block[1]), k - 1);
    }
    mpz_clears(ntemp, m, NULL);
    free(block);
    block = NULL;
}
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n);
}
// Decrypt encrypted data in infile using n and d.
// Write decrypted data to outfile.
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    uint64_t k = 0;
    int fd = fileno(outfile);
    mpz_t ntemp, c;
    mpz_init_set(ntemp, n);
    mpz_init(c);
    while (mpz_get_ui(ntemp) > 0) {
        k++;
        mpz_fdiv_q_ui(ntemp, ntemp, 2);
    }
    k = (k - 1) / 8;
    uint8_t *block = (uint8_t *) calloc(k, sizeof(uint8_t));
    int reading = gmp_fscanf(infile, "%Zx\n", c);
    size_t j;
    while (reading == 1) {
        rsa_decrypt(ntemp, c, d, n);
        mpz_export(block, &j, 1, 1, 1, 0, ntemp);
        write(fd, &(block[1]), j - 1);
        reading = gmp_fscanf(infile, "%Zx\n", c);
    }
    mpz_clears(ntemp, c, NULL);
    free(block);
    block = NULL;
}
// Sign a username m with d and n.
// Verify a signature s of a username m with e and n.
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d, n);
}
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    mpz_t t;
    mpz_init(t);
    pow_mod(t, s, e, n);
    int comp = mpz_cmp(m, t);
    mpz_clear(t);
    return comp == 0;
}
