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
   * \param t simulation temperature
   * \param p_lattice_list lattice list
   * \param p_counter lattices and atoms counter
   */
  MEventHook(const conf::Output &output_config, const double t, LatticesList *p_lattice_list, counter *p_counter);

  void onStepFinished(const unsigned long step, const double delta_MC_time) override;

  void onAllDone() override;

private:
  double real_phy_time = 0.0;
  double total_mc_time = 0.0;
  const double temperature = 0.0;
  /**
   * \brief output config
   */
  const conf::Output output_config;
  LatticesList *p_lattice_list = nullptr;
  counter *p_counter = nullptr;
};

#endif // MISA_KMC_M_EVENT_HOOK_H
