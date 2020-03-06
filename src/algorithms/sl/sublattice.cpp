//
// Created by genshen on 2019/9/21.
//

#include "sublattice.h"
#include <comm/preset/sector_forwarding_region.h>

SectorMeta::SectorMeta() : sector_itl(sectors.begin()) {}

SubLattice::SubLattice(const comm::ColoredDomain *p_domain,
                       const uint32_t seed_time_inc, const double time_limit,
                       const double T)
    : time_inc_dis(0, 1), p_domain(p_domain), T(T), time_limit(time_limit),
      sec_meta() {
  // note: time_inc_dis can produces random numbers in a range [0, 1) todo
  // skip random number 0

  // initialize a rng using the given seed
  time_inc_rng.seed(seed_time_inc);
}

void SubLattice::nextSector() {}
