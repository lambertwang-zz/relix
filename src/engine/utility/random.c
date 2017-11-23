#include "random.h"

#include <stdlib.h>
#include <time.h>

unsigned int drand_seed;

int drandom_i(int min, int max) {
    return min + drandom_f() * (max - min);
}

float drandom_f() {
    srand(drand_seed);
    drand_seed = rand();

    return (float)drand_seed / (float)RAND_MAX;
}

void initRandom_s(unsigned int seed) {
	srand(seed);
    drand_seed = seed;
}

void initRandom() {
	srand(time(0));
    drand_seed = rand();
}

// Non-deterministic random number generation
int nrandom_i(int min, int max) {
    return min + nrandom_f() * (max - min);
}

float nrandom_f() {
    return (float)rand() / (float)RAND_MAX;
}
