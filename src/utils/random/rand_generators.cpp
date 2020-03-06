//
// Created by genshen on 2019-02-10.
//

#include "rand_generators.h"

#ifndef RAND_LINUX_REAL
r::type_rng r::rng;

void r::seed(const uint32_t seed) { rng.seed(seed); }

uint32_t r::rand32() { return rng(); }

#else
// todo
#endif
