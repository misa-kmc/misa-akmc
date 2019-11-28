//
// Created by genshen on 2019/10/30.
//

#include <gtest/gtest.h>
#include <comm/domain/colored_domain.h>
#include <utils/mpi_utils.h>
#include <utils/simulation_domain.h>
#include <lattice/normal_lattice_list.h>
#include <utils/macros.h>
#include <pack/ghost_init_packer.h>
#include <utils/mpi_types.h>

class TestNormalLatticeList : public NormalLatticeList {
public:
    TestNormalLatticeList(comm::_type_lattice_size box_size_x, comm::_type_lattice_size box_size_y,
                          comm::_type_lattice_size box_size_z, comm::_type_lattice_size ghost_size_x,
                          comm::_type_lattice_size ghost_size_y, comm::_type_lattice_size ghost_size_z)
            : NormalLatticeList(box_size_x, box_size_y, box_size_z,
                                ghost_size_x, ghost_size_y, ghost_size_z) {}

    FRIEND_TEST(ghost_init_packer_id_test, ghost_init_packer_test);
};

TEST(ghost_init_packer_id_test, ghost_init_packer_test) {
    // set domain
    const int64_t phase_space_int64[comm::DIMENSION_SIZE] = {40, 40, 40};
    const double cutoff_radius = 5.6;
    const double lattice_const = 2.85532;

    // set domain
    MPI_Comm new_comm;
    comm::mpi_process pro = comm::mpi_process{
            kiwi::mpiUtils::local_process.own_rank,
            kiwi::mpiUtils::local_process.all_ranks,
            kiwi::mpiUtils::local_process.comm,
    };
    auto _p_domain = comm::ColoredDomain::Builder()
            .setComm(pro, &new_comm)
            .setPhaseSpace(phase_space_int64)
            .setCutoffRadius(cutoff_radius)
            .setLatticeConst(lattice_const)
            .build();
    kiwi::mpiUtils::onGlobalCommChanged(new_comm);
    SimulationDomain::setSimDomain(kiwi::mpiUtils::global_process);

    // set lattice list
    auto lattice_list = new TestNormalLatticeList{
            _p_domain->sub_box_lattice_size[0], _p_domain->sub_box_lattice_size[1],
            _p_domain->sub_box_lattice_size[0], _p_domain->lattice_size_ghost[0],
            _p_domain->lattice_size_ghost[1], _p_domain->lattice_size_ghost[2]
    };
    // set lattice id.
    for (comm::_type_lattice_coord z = 0; z < _p_domain->local_ghost_ext_lattice_region.z_high; z++) {
        for (comm::_type_lattice_coord y = 0; y < _p_domain->local_ghost_ext_lattice_region.y_high; y++) {
            // due to BCC, x is doubled.
            for (comm::_type_lattice_coord x = 0; x < BCC_DBX * _p_domain->local_ghost_ext_lattice_region.x_high; x++) {
                if (_p_domain->local_sub_box_lattice_region.isIn(x / BCC_DBX, y, z)) { // if it is in simulation box
                    lattice_list->_lattices[z][y][x].id = 0x200;
                } else { // ghost area.
                    lattice_list->_lattices[z][y][x].id = 0x100;
                }
            }
        }
    }

    //  establish ghost area by communicating for all sectors.
    mpi_types::setInterMPIType();
    GhostInitPacker init_packer{_p_domain, lattice_list};
    comm::neiSendReceive(&init_packer, SimulationDomain::comm_sim_pro,
                         mpi_types::_mpi_type_lattice_data,
                         _p_domain->rank_id_neighbours);
    mpi_types::unsetInterMPIType();

    // test lattice in ghost area
    for (comm::_type_lattice_coord z = 0; z < _p_domain->local_ghost_ext_lattice_region.z_high; z++) {
        for (comm::_type_lattice_coord y = 0; y < _p_domain->local_ghost_ext_lattice_region.y_high; y++) {
            // due to BCC, x is doubled.
            for (comm::_type_lattice_coord x = 0; x < BCC_DBX * _p_domain->local_ghost_ext_lattice_region.x_high; x++) {
                // all lattice id will be 200, not only simulation area.
                EXPECT_EQ(lattice_list->_lattices[z][y][x].id, 0x200);
            }
        }
    }

    delete lattice_list;
    delete _p_domain;
}
