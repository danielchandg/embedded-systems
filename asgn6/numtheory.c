#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>

#include "numtheory.h"
#include "randstate.h"

// Parameters: (output, a, b).
void gcd(mpz_t g, mpz_t a, mpz_t b) {
    mpz_t a1, b1;
    mpz_init_set(a1, a);
    mpz_init_set(b1, b);
    while (mpz_cmp_si(b1, 0) != 0) {
        mpz_set(g, b1);
        mpz_mod(b1, a1, b1);
        mpz_set(a1, g);
    }
    mpz_set(g, a1);
    mpz_clears(a1, b1, NULL);
}
void mod_inverse(mpz_t o, mpz_t a, mpz_t n) {
    // r = r, r' = r1, t = o, t' = t1
    mpz_t r, r1, t1, q, temp;
    mpz_init_set(r, n);
    mpz_init_set(r1, a);
    mpz_set_si(o, 0);
    mpz_init_set_si(t1, 1);
    mpz_inits(q, temp, NULL);
    while (mpz_cmp_si(r1, 0) != 0) {
        //gmp_printf("r' = %Zd\n", r1);
        // q = floor(r / r')
        mpz_fdiv_q(q, r, r1);

        // [r, r'] = [r', r - q*r']
        mpz_mul(temp, q, r1);
        mpz_sub(temp, r, temp);
        mpz_set(r, r1);
        mpz_set(r1, temp);

        // [t, t'] = [t', t - q*t']
        mpz_mul(temp, q, t1);
        mpz_sub(temp, o, temp);
        mpz_set(o, t1);
        mpz_set(t1, temp);
    }
    if (mpz_cmp_si(r, 1) > 0) {
        mpz_set_si(o, 0);
    } else if (mpz_cmp_si(o, 0) < 0) {
        mpz_add(o, o, n);
    }
    mpz_clears(r, r1, t1, q, temp, NULL);
}
// Parameters: (output, base, exponent, modulus).
// Make sure a, d, and n are all positive.
void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) {
    mpz_t p, exp, v;
    mpz_init_set_ui(v, 1);
    mpz_init_set(p, a);
    mpz_init_set(exp, d);
    while (mpz_cmp_si(d, 0) > 0) {
        // if d is odd, set v to (v*p)%n.
        if (mpz_fdiv_ui(d, 2) == 1) {
            mpz_mul(v, v, p);
            mpz_mod(v, v, n);
        }
        // p = (p*p)%n
        mpz_mul(p, p, p);
        mpz_mod(p, p, n);
        // d = floor(d/2)
        mpz_fdiv_q_ui(exp, exp, 2);
    }
    mpz_set(d, exp);
    mpz_set(o, v);
    mpz_clears(p, exp, v, NULL);
}
bool is_prime(mpz_t n, uint64_t iters) {
    //gmp_randstate_t state;
    //gmp_randinit_mt(state);
    //gmp_randseed_ui(state, 1);

    // Check if n is even.
    if (mpz_fdiv_ui(n, 2) == 0)
        return false;
    if(mpz_cmp_si(n, 0) < 0) return false;
    mpz_t s, r, a, y, j, two, upper, temp, n_1;
    mpz_init_set(r, n);
    mpz_init_set(n_1, n);
    mpz_init_set_ui(s, 0);
    mpz_sub_ui(r, r, 1);
    mpz_sub_ui(n_1, n_1, 1);
    mpz_init_set(upper, n);
    mpz_sub_ui(upper, upper, 3);
    // s = 0, r = n-1.
    // While r is even, halve r and increment s.
    while (mpz_cmp_ui(r, 0) != 0 && mpz_fdiv_ui(r, 2) == 0) {
        mpz_fdiv_q_ui(r, r, 2);
        mpz_add_ui(s, s, 1);
    }
    //printf("%lu-1 = 2^%lu * %lu\n", mpz_get_ui(n), mpz_get_ui(s), mpz_get_ui(r));
    mpz_inits(a, y, j, temp, NULL);
    mpz_init_set_ui(two, 2);
    for (uint64_t i = 0; i < iters; i++) {
        //printf("Iter #%lu\n", i);
        mpz_urandomm(a, state, upper);
        mpz_add_ui(a, a, 2);
        //printf("Rand from 2 to n-2: %lu\n", mpz_get_ui(a));
        pow_mod(y, a, r, n);
        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, n_1) != 0) {
            mpz_set_ui(j, 1);
            while (mpz_cmp(j, s) < 0 && mpz_cmp(y, n_1) != 0) {
                //printf("y=%lu, n=%lu, two=%lu\n", mpz_get_ui(y), mpz_get_ui(n), mpz_get_ui(two));
                pow_mod(temp, y, two, n);
                mpz_set(y, temp);
                //printf("y^2=%lu\n", mpz_get_ui(y));
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(s, r, a, y, j, two, upper, temp, n_1, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, n_1) != 0) {
                mpz_clears(s, r, a, y, j, two, upper, temp, n_1, NULL);
                return false;
            }
        }
    }
    mpz_clears(s, r, a, y, j, two, upper, temp, n_1, NULL);
    return true;
}
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    //gmp_randstate_t state;
    //gmp_randinit_mt(state);
    //gmp_randseed_ui(state, 1);

    mpz_t base;
    mpz_init_set_ui(base, 1);
    for (uint64_t i = 0; i < bits - 1; i++) {
        mpz_mul_ui(base, base, 2);
    }
    //printf("base: %lu\n", mpz_get_ui(base));
    do {
        mpz_urandomb(p, state, bits - 1);
        mpz_add(p, p, base);
        // gmp_printf("p: %Zd\n", p);
    } while (!is_prime(p, iters));
    mpz_clear(base);
}
