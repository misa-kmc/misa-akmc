//
// Created by z52527 on 2019-09-24.
//

#include "simulation.h"
#include "utils/mpi_utils.h"

void simulation::createDomain(const unsigned long phase_space[comm::DIMENSION_SIZE],
                              const double lattice_const, const double cutoff_radius) {

    //进行区域分解
//    kiwi::logs::v(MASTER_PROCESSOR, "domain", "Initializing GlobalDomain decomposition.\n");

    MPI_Comm new_comm;
    comm::mpi_process pro = comm::mpi_process{
            kiwi::mpiUtils::local_process.own_rank,
            kiwi::mpiUtils::local_process.all_ranks,
            kiwi::mpiUtils::local_process.comm,
    };
    // In domain creation, we set ghost size as (cut_lattice +1) to avoid neighbor index overflowing.
    int64_t phase_space_int64[comm::DIMENSION_SIZE];
    for(int i = 0; i < comm::DIMENSION_SIZE; i++)
        phase_space_int64[i] = (int64_t)phase_space[i];
    _p_domain = comm::ColoredDomain::Builder()
            .setComm(pro, &new_comm)
            .setPhaseSpace(phase_space_int64)
            .setCutoffRadius(cutoff_radius)
            .setLatticeConst(lattice_const)
//            .setGhostSize(static_cast<int>(ceil(cutoff_radius)))
            .build();
    kiwi::mpiUtils::onGlobalCommChanged(new_comm); // set new domain.
//    MPIDomain::sim_processor = kiwi::mpiUtils::global_process;

//    kiwi::logs::v(MASTER_PROCESSOR, "domain", "Initialization done.\n");
}
