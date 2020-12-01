//
// Created by genshen on 2019/12/10.
//

#ifndef MISA_KMC_SEED_RELOCATE_H
#define MISA_KMC_SEED_RELOCATE_H

#include <cstdint>
#include <logs/logs.h>
#include <random>
#include <utils/random/random.h>

namespace r {
  /**
   * \brief if the seed is 0, set the seed using random device.
   */
  void seedRelocate(uint32_t *create_types, uint32_t *create_vacancy, uint32_t *event_selection, uint32_t *time_inc) {
    std::random_device rd;
    if (*create_types == r::seed_auto) {
      *create_types = rd();
      kiwi::logs::i("random", "create_types seed: {}\n", *create_types);
    }
    if (*create_vacancy == r::seed_auto) {
      *create_vacancy = rd();
      kiwi::logs::i("random", "create_vacancy seed: {}\n", *create_vacancy);
    }
    if (*event_selection == r::seed_auto) {
      *event_selection = rd();
      kiwi::logs::i("random", "event_selection seed: {}\n", *event_selection);
    }
    if (*time_inc == r::seed_auto) {
      *time_inc = rd();
      kiwi::logs::i("random", "time_inc seed: {}\n", *time_inc);
    }
  }
} // namespace r

#endif // MISA_KMC_SEED_RELOCATE_H
