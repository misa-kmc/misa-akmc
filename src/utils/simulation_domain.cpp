//
// Created by genshen on 2019/10/23.
//

#include "simulation_domain.h"

// the MPI ranks metadata,
// with the same content, but different types.
kiwi::mpi_process SimulationDomain::kiwi_sim_pro;
comm::mpi_process SimulationDomain::comm_sim_pro;

void SimulationDomain::setSimDomain(kiwi::mpi_process p) {
  kiwi_sim_pro = p;
  comm_sim_pro = comm::mpi_process{
      .own_rank = p.own_rank,
      .all_ranks = p.all_ranks,
      .comm = p.comm,
  };
}
