//
// Created by genshen on 2019-02-10.
//

#ifndef MISA_KMC_RAND_GENERATOR_H
#define MISA_KMC_RAND_GENERATOR_H

#include <cstdint>
#include <random>

#include <xoshiro_gen.h>
#include "../../building_config.h"

namespace r {
#ifdef RAND_LCG
    extern std::minstd_rand rng;

    /**
     * set seed for rng
     */
    void seed(const uint32_t seed);

    /**
     * \brief returns a uint32_t random type, between \param low to \param high.
     */
    uint32_t rand32();
#endif

#ifdef RAND_MT
    extern std::mt19937 rng;

    /**
     * set seed for rng
     */
    void seed(const uint32_t seed);

    /**
     * \brief returns a uint32_t random type, between \param low to \param high.
     */
    uint32_t rand32();
#endif

#ifdef RAND_STC
    extern std::ranlux24 rng;

    /**
     * set seed for rng
     */
    void seed(const uint32_t seed);

    /**
     * \brief returns a uint32_t random type, between \param low to \param high.
     */
    uint32_t rand32();
#endif

#ifdef RAND_XOSHIRO
    // return type is uint32_t for 128;
    // return type is uint64_t for 256;
    extern util::random::xoroshiro128_plus rng;

    /**
     * set seed for rng
     */
    void seed(const uint32_t seed);

    /**
     * \brief returns a uint32_t random type, between \param low to \param high.
     */
    uint32_t rand32();
#endif

#ifdef RAND_LINUX_REAL
#endif
}

#endif //MISA_KMC_RAND_GENERATOR_H
