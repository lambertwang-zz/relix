#ifndef __RANDOM_H__
#define __RANDOM_H__

void initRandom_s(unsigned int seed);
void initRandom();

// Deterministic rng
// Returns a number = [min..max]
int drandom_i(int min, int max);
// Returns a number = (0.0..1.0)
float drandom_f();

// Non-deterministic rng
int nrandom_i(int min, int max);
float nrandom_f();

#endif

