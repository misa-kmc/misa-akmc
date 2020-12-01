//
// Created by genshen on 2019-02-10.
//

#ifndef MISA_KMC_RANDOM__H
#define MISA_KMC_RANDOM__H

#include <chrono>
#include <cstdint>

namespace r {
  const uint32_t seed_auto = 0;

  /**
   * \brief set seed for rng.
   * \param seed random seed to be used. if \param seed equals seed_auto and it is
   * not debug mode, seed will be initialized by std::random_device. \return the
   * seed used for rng
   */
  uint32_t initSeed(const uint32_t seed);

  /**
   * \brief \brief returns a uint32_t random type, between \param low to \param
   * high. \param low \param high \return
   */
  uint32_t rand32(const uint32_t low, const uint32_t high);

  /**
   * \brief returns a double random type, between 0 to 1.
   * \return random data
   */
  double random();
}; // namespace r

#endif // MISA_KMC_RANDOM__H
