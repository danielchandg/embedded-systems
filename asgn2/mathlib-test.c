#include "bbp.c"
#include "e.c"
#include "euler.c"
#include "madhava.c"
#include "mathlib.h"
#include "newton.c"
#include "viete.c"

#include <math.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "aebmrvnsh"

void help_string(char *cwd) {
    printf("SYNOPSIS\n   A test harness for the small numerical library.\n\nUSAGE\n   "
           "%s/mathlib-test [-aebmrvnsh]\n\nOPTIONS\n  -a   Runs all tests.\n  -e   Runs e test.\n "
           " -b   Runs BBP pi test.\n  -m   Runs Madhava pi test.\n  -r   Runs Euler pi test.\n  "
           "-v   Runs Viete pi test.\n  -n   Runs Newton square root tests.\n  -s   Print verbose "
           "statistics.\n  -h   Display program synopsis and usage.\n",
        cwd);
}

int main(int argc, char **argv) {
    int opt = 0, args[] = { 0, 0, 0, 0, 0, 0, 0 };
    char cwd[FILENAME_MAX];
    getcwd(cwd, FILENAME_MAX);
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            for (int i = 0; i < 6; i++)
                args[i] = 1;
            break;
        case 'e': args[0] = 1; break;
        case 'b': args[1] = 1; break;
        case 'm': args[2] = 1; break;
        case 'r': args[3] = 1; break;
        case 'v': args[4] = 1; break;
        case 'n': args[5] = 1; break;
        case 's': args[6] = 1; break;
        case 'h':
            printf("H\n");
            help_string(cwd);
            return 0;
        }
    }
    // printf("Args: %d, %d, %d, %d, %d, %d, %d\n", args[0],args[1],args[2],args[3],args[4],args[5],args[6]);
    if (args[0] + args[1] + args[2] + args[3] + args[4] + args[5] == 0) {
        help_string(cwd);
        return 0;
    }
    double e1, pi;
    if (args[0]) {
        e1 = e();
        printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", e1, M_E, absolute(e1 - M_E));
        if (args[6])
            printf("e() terms = %d\n", e_terms());
    }
    if (args[3]) {
        pi = pi_euler();
        printf("pi_euler() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi, M_PI,
            absolute(pi - M_PI));
        if (args[6])
            printf("pi_euler() terms = %d\n", pi_euler_terms());
    }
    if (args[1]) {
        pi = pi_bbp();
        printf("pi_bbp() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi, M_PI,
            absolute(pi - M_PI));
        if (args[6])
            printf("pi_bbp() terms = %d\n", pi_bbp_terms());
    }
    if (args[2]) {
        pi = pi_madhava();
        printf("pi_madhava() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi, M_PI,
            absolute(pi - M_PI));
        if (args[6])
            printf("pi_madhava() terms = %d\n", pi_madhava_terms());
    }
    if (args[4]) {
        pi = pi_viete();
        printf("pi_viete() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi, M_PI,
            absolute(pi - M_PI));
        if (args[6])
            printf("pi_viete() terms = %d\n", pi_viete_factors());
    }
    if (args[5]) {
        for (double i = 0; i <= 10; i += 0.1) {
            double sq1 = sqrt_newton(i), sq2 = sqrt(i);
            printf("sqrt_newton(%7.6lf) = %16.15lf, sqrt(%7.6lf) = %16.15lf, diff = %16.15lf\n", i,
                sq1, i, sq2, absolute(sq1 - sq2));
            if (args[6])
                printf("sqrt_newton() terms = %d\n", sqrt_newton_iters());
        }
    }
    return 0;
}
