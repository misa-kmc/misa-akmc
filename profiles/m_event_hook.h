//
// Created by genshen on 2019/12/20.
//

#ifndef MISA_KMC_M_EVENT_HOOK_H
#define MISA_KMC_M_EVENT_HOOK_H

#include "config/config_values.h"
#include <counter.h>
#include <hook/event_hooks.hpp>
#include <lattice/lattices_list.h>

class MEventHook : public EventHooks {
public:
  /**
   * \brief initialize hook by lattice list and counter
   * \param p_lattice_list lattice list
   * \param p_counter lattices and atoms counter
   */
  MEventHook(const conf::Output &output_config, LatticesList *p_lattice_list, counter *p_counter);

  void onStepFinished(unsigned long step) override;

  void onAllDone() override;

private:
  /**
   * \brief output config
   */
  const conf::Output output_config;
  LatticesList *p_lattice_list = nullptr;
  counter *p_counter = nullptr;
};

#endif // MISA_KMC_M_EVENT_HOOK_H
