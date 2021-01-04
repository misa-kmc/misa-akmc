//
// Created by genshen on 2019-02-10.
//

#ifndef MISA_KMC_RAND_GENERATOR_H
#define MISA_KMC_RAND_GENERATOR_H

#include <cstdint>
#include <random>

#include "../../building_config.h"
#include "rng_type.h"
#include <xoshiro_gen.h>

namespace r {
#ifndef RAND_LINUX_REAL

  extern type_rng rng;

  /**
   * set seed for rng
   */
  void seed(const uint32_t seed);

  /**
   * \brief returns a uint32_t random type, between \param low to \param high.
   */
  uint32_t rand32();

#else
// todo
#endif

} // namespace r

#endif // MISA_KMC_RAND_GENERATOR_H
