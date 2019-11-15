//
// Created by runchu on 2019-09-24.
//

#include <utils/mpi_utils.h>
#include "utils/mpi_types.h"
#include "simulation.h"
#include "algorithms/sl/sublattice.h"
#include "models/abvi/abvi_model.h"
#include "pack/ghost_init_packer.h"
#include "pack/packer_instance.h"
#include "pack/ghost_sync_packer.h"
#include "utils/simulation_domain.h"

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
    for (int i = 0; i < comm::DIMENSION_SIZE; i++) {
        phase_space_int64[i] = (int64_t) phase_space[i];
    }
    _p_domain = comm::ColoredDomain::Builder()
            .setComm(pro, &new_comm)
            .setPhaseSpace(phase_space_int64)
            .setCutoffRadius(cutoff_radius)
            .setLatticeConst(lattice_const)
//            .setGhostSize(static_cast<int>(ceil(cutoff_radius)))
            .build();
    kiwi::mpiUtils::onGlobalCommChanged(new_comm); // set new domain.
    SimulationDomain::setSimDomain(kiwi::mpiUtils::global_process);

//    kiwi::logs::v(MASTER_PROCESSOR, "domain", "Initialization done.\n");
}

void simulation::createLattice() {
    // create empty lattice list.
    // todo type conversion.
    // todo delete lattice list
    lattice_list = new NormalLatticeList{static_cast<_type_box_size>(_p_domain->sub_box_lattice_size[0]),
                                         static_cast<_type_box_size>(_p_domain->sub_box_lattice_size[1]),
                                         static_cast<_type_box_size>(_p_domain->sub_box_lattice_size[0]),
                                         static_cast<_type_box_size>(_p_domain->lattice_size_ghost[0]),
                                         static_cast<_type_box_size>(_p_domain->lattice_size_ghost[1]),
                                         static_cast<_type_box_size>(_p_domain->lattice_size_ghost[2])
    };
}

void simulation::prepareForStart() {
    // initialize ghost area for all sectors.
    GhostInitPacker init_packer{_p_domain, lattice_list};
    comm::neiSendReceive(&init_packer, SimulationDomain::comm_sim_pro,
                         mpi_types::_mpi_type_lattice_data,
                         _p_domain->rank_id_neighbours);
}

void simulation::simulate(const double time_limit) {
    ABVIModel model;
    SubLattice sl(_p_domain, &model, time_limit, 1.0); // todo calculate T

    PackerInstance pk_ins(lattice_list);
    // todo use other packers: simulation sync packer.
    sl.startTimeLoop<GhostSyncPacker, GhostSyncPacker, PackerInstance>(pk_ins);
}
