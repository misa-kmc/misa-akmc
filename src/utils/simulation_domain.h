//
// Created by genshen on 2019/10/23.
//

#ifndef MISA_KMC_SIMULATION_DOMAIN_H
#define MISA_KMC_SIMULATION_DOMAIN_H

#include <comm/types_define.h>
#include <utils/mpi_utils.h>

struct SimulationDomain {
  // the MPI ranks metadata,
  // with the same content, but different types.
  static kiwi::mpi_process kiwi_sim_pro;
  static comm::mpi_process comm_sim_pro;

  static void setSimDomain(kiwi::mpi_process p);
};

#endif // MISA_KMC_SIMULATION_DOMAIN_H
