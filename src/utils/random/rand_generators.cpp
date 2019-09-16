//
// Created by genshen on 2019-02-10.
//

#include "rand_generators.h"


#ifdef RAND_LCG
std::minstd_rand r::rng;

void r::seed(const uint32_t seed){
    rng.seed(seed);
}

uint32_t r::rand32(){
     return rng();
}

#endif

#ifdef RAND_MT
std::mt19937 r::rng;

void r::seed(const uint32_t seed){
    rng.seed(seed);
}

uint32_t r::rand32(){
     return rng();
}

#endif

#ifdef RAND_STC
std::ranlux24 r::rng;

void r::seed(const uint32_t seed){
    rng.seed(seed);
}

uint32_t r::rand32(){
     return rng();
}

#endif

#ifdef RAND_XOSHIRO
util::random::xoroshiro128_plus r::rng;

void r::seed(const uint32_t seed){
     rng.init_seed(seed);
}

uint32_t r::rand32(){
     return rng();
}

#endif

#ifdef RAND_LINUX_REAL
#endif