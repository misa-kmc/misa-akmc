//
// Created by genshen on 2019/12/5.
//

#ifndef MISA_KMC_RNG_TYPE_H
#define MISA_KMC_RNG_TYPE_H

#include "building_config.h"
#include <random>
#include <xoshiro_gen.h>

namespace r {
#ifdef RAND_LCG
  typedef std::minstd_rand type_rng;
#endif

#ifdef RAND_MT
  typedef std::mt19937 type_rng;
#endif

#ifdef RAND_STC
  typedef std::ranlux24 type_rng;
#endif

#ifdef RAND_XOSHIRO
  // return type is uint32_t for 128;
  // return type is uint64_t for 256;
  typedef util::random::xoroshiro128_plus type_rng;
#endif

#ifdef RAND_LINUX_REAL
// todo
#endif

} // namespace r

#endif // MISA_KMC_RNG_TYPE_H
