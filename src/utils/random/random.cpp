//
// Created by genshen on 2019-02-10.
//

#include <random>
#include "random.h"

#ifndef DEBUG_MODE

#include "rand_generators.h"

#endif

void r::initSeed() {
#ifndef DEBUG_MODE
    std::random_device rd;
    r::seed(rd());
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
//    return (r >> 11) * (1.0 / (UINT64_C(1) << 53));
    return rand32() * (1.0 / 0xFFFFFFFFL);
#endif
}
