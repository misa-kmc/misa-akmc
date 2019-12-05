//
// Created by genshen on 2019-02-10.
//

#include <random>
#include "random.h"

#ifndef DEBUG_MODE

#include "rand_generators.h"

#endif

uint32_t r::initSeed(const uint32_t seed) {
#ifdef DEBUG_MODE
    srand(seed);
    return seed;
#else
    if (seed == seed_auto) {
        std::random_device rd;
        const uint32_t seed_ = rd();
        r::seed(seed_);
        return seed_;
    } else {
        r::seed(seed);
        return seed;
    }
#endif
}

uint32_t r::rand32(const uint32_t low, const uint32_t high) {
#ifdef DEBUG_MODE
    return rand() % (high - low) + low;
#else
    return rand32() % (high - low) + low;
#endif
}

double r::random() {
#ifdef DEBUG_MODE
    return (double)rand() / RAND_MAX;
#else
    return rand32() * (1.0 / r::type_rng::max());
#endif
}
