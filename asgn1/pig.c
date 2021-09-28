#include "names.h"

#include <stdio.h>
#include <stdlib.h>

typedef enum { SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER } Position;
const Position pig[7] = { SIDE, SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER, JOWLER };

int main(void) {
    int players = -1, seed = -1, turn = 0, values[5] = { 0, 10, 10, 15, 5 }, pts[10];
    const char *m[5];
    m[0] = "on side", m[1] = "on back", m[2] = "upright", m[3] = "on snout", m[4] = "on ear";
    printf("How many players? ");
    scanf("%d", &players);
    if (players < 2 || players > 10) {
        fprintf(stderr, "Invalid number of players. Using 2 instead.\n");
        players = 2;
    }
    printf("Random seed: ");
    scanf("%d", &seed);
    if (seed < 0) {
        fprintf(stderr, "Invalid random seed. Using 2021 instead.\n");
        seed = 2021;
    }
    srandom(seed);
    for (int i = 0; i < players; i += 1)
        pts[i] = 0;
    while (1) {
        printf("%s rolls the pig... ", names[turn]);
        int roll = random() % 7;
        pts[turn] += values[pig[roll]];
        printf("pig lands %s ", m[pig[roll]]);
        while (pig[roll] != SIDE && pts[turn] < 100) {
            roll = random() % 7;
            pts[turn] += values[pig[roll]];
            printf("pig lands %s ", m[pig[roll]]);
        }
        if (pts[turn] >= 100) {
            printf("\n%s wins with %d points!\n", names[turn], pts[turn]);
            break;
        }
        turn = (turn + 1) % players;
        printf("\n");
    }
    return 0;
}
