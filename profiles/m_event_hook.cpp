//
// Created by genshen on 2019/12/20.
//

#include "m_event_hook.h"

#include "analysis/ans.h"
#include "io/lattice_dump.h"
#include "real_time.hpp"
#include "utils/simulation_domain.h"
#include <logs/logs.h>
#include <string>

MEventHook::MEventHook(const conf::Output &output_conf, const double t, LatticesList *p_lattice_list,
                       counter *p_counter)
    : output_config(output_conf), temperature(t), p_lattice_list(p_lattice_list), p_counter(p_counter) {}

void MEventHook::onStepFinished(const unsigned long step, const double delta_MC_time) {
  ans::analysis_result result = ans::analysis(p_lattice_list);

  const double sim_C_V = static_cast<double>(result.vacancies) / static_cast<double>(result.lattice_num);

  // the number of isolated Cu / total lattice in global simulation box.
  const double c_x = static_cast<double>(result.isolated_solutes) / static_cast<double>(result.lattice_num);
  const double real_dt = kmc::convert_to_real_time(sim_C_V, delta_MC_time, temperature, c_x);
  real_phy_time += real_dt;
  total_mc_time += delta_MC_time;

  if (step % 200 == 0) {
    kiwi::logs::d("hook", "finish step: {}, MC time: {}, phy_time: {}\n", step, total_mc_time, real_phy_time);
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
