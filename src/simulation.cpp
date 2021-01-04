//
// Created by runchu on 2019-09-24.
//

#include "simulation.h"
#include "abvi/kmc.h"
#include "pack/ghost_init_packer.h"
#include "utils/mpi_types.h"
#include "utils/simulation_domain.h"
#include <utils/mpi_utils.h>

void simulation::createDomain(const unsigned long phase_space[comm::DIMENSION_SIZE], const double lattice_const,
                              const double cutoff_radius) {

  //进行区域分解
  //    kiwi::logs::v(MASTER_PROCESSOR, "domain", "Initializing GlobalDomain
  //    decomposition.\n");

  MPI_Comm new_comm;
  comm::mpi_process pro = comm::mpi_process{
      kiwi::mpiUtils::local_process.own_rank,
      kiwi::mpiUtils::local_process.all_ranks,
      kiwi::mpiUtils::local_process.comm,
  };
  // In domain creation, we set ghost size as (cut_lattice +1) to avoid
  // neighbor index overflowing.
  int64_t phase_space_int64[comm::DIMENSION_SIZE];
  for (int i = 0; i < comm::DIMENSION_SIZE; i++) {
    phase_space_int64[i] = (int64_t)phase_space[i];
  }
  _p_domain = comm::ColoredDomain::Builder()
                  .setComm(pro, &new_comm)
                  .setPhaseSpace(phase_space_int64)
                  .setCutoffRadius(cutoff_radius / lattice_const)
                  .setLatticeConst(lattice_const)
                  //            .setGhostSize(static_cast<int>(ceil(cutoff_radius/lattice_const)))
                  .build();
  kiwi::mpiUtils::onGlobalCommChanged(new_comm); // set new domain.
  SimulationDomain::setSimDomain(kiwi::mpiUtils::global_process);

  //    kiwi::logs::v(MASTER_PROCESSOR, "domain", "Initialization done.\n");
}

void simulation::createLattice() {
  BoxBuilder builder{};
  // todo type conversion.
  builder.setBoxSize(static_cast<_type_box_size>(_p_domain->sub_box_lattice_size[0]),
                     static_cast<_type_box_size>(_p_domain->sub_box_lattice_size[1]),
                     static_cast<_type_box_size>(_p_domain->sub_box_lattice_size[2]));
  builder.setGhostSize(static_cast<_type_box_size>(_p_domain->lattice_size_ghost[0]),
                       static_cast<_type_box_size>(_p_domain->lattice_size_ghost[1]),
                       static_cast<_type_box_size>(_p_domain->lattice_size_ghost[2]));

  builder.setGlobalLatSize(_p_domain->phase_space[0], _p_domain->phase_space[1], _p_domain->phase_space[2]);
  builder.setGlobalBaseLat(_p_domain->sub_box_lattice_region.x_low, _p_domain->sub_box_lattice_region.y_low,
                           _p_domain->sub_box_lattice_region.z_low);
  // create empty lattice list(lattice types are not specified) and defects
  // list.
  box = builder.build(); // todo delete pointer
}

void simulation::prepareForStart() {
  // initialize ghost area for all sectors.
  GhostInitPacker init_packer{_p_domain, box->lattice_list};
  comm::neiSendReceive(&init_packer, SimulationDomain::comm_sim_pro, mpi_types::_mpi_type_lattice_data,
                       _p_domain->rank_id_neighbours);
}
