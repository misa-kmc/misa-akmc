//
// Created by genshen on 2019/12/20.
//

#include "m_event_hook.h"
#include "io/lattice_dump.h"
#include "utils/simulation_domain.h"
#include <logs/logs.h>
#include <string>

MEventHook::MEventHook(const conf::Output &output_conf, LatticesList *p_lattice_list, counter *p_counter)
    : output_config(output_conf), p_lattice_list(p_lattice_list), p_counter(p_counter) {}

void MEventHook::onStepFinished(unsigned long step) {
  if (step % 200 == 0) {
    kiwi::logs::d("hook", "finish step: {}\n", step);
  }
  if (step % output_config.dump_interval == 0) {
    std::string dump_file_path = fmt::format(output_config.dump_file_path, step);
    const kiwi::mpi_process process = SimulationDomain::kiwi_sim_pro;
    if (process.all_ranks != 1) {
      dump_file_path = fmt::format(output_config.dump_file_path + ".{}", step, process.own_rank);
    }
    kiwi::logs::v("dump", "dumping to file {} at step {}.\n", dump_file_path, step);
    LatticeDump dump;
    dump.dump(dump_file_path, p_lattice_list, step);
  }
}

void MEventHook::onAllDone() {}
