//
// Created by genshen on 2019/10/30.
//

#include "../domain_test_fixtures.h"
#include <comm/domain/colored_domain.h>
#include <gtest/gtest.h>
#include <lattice/normal_lattice_list.h>
#include <pack/ghost_init_packer.h>
#include <utils/macros.h>
#include <utils/mpi_types.h>
#include <utils/mpi_utils.h>
#include <utils/simulation_domain.h>

class TestPeriodLatticeList : public PeriodLatticeList {
public:
  explicit TestPeriodLatticeList(LatListMeta meta) : PeriodLatticeList(meta) {}

  FRIEND_TEST(GhostInitPackerTest, ghost_init_packer_test);
};

// test fixture class
class GhostInitPackerTest : public DomainSteUp<TestPeriodLatticeList> {};

// @MPI
TEST_F(GhostInitPackerTest, ghost_init_packer_test) {
  // set lattice id.
  for (comm::_type_lattice_coord z = 0; z < p_domain->local_ghost_ext_lattice_region.z_high; z++) {
    for (comm::_type_lattice_coord y = 0; y < p_domain->local_ghost_ext_lattice_region.y_high; y++) {
      // due to BCC, x is doubled.
      for (comm::_type_lattice_coord x = 0; x < BCC_DBX * p_domain->local_ghost_ext_lattice_region.x_high; x++) {
        if (p_domain->local_sub_box_lattice_region.isIn(x / BCC_DBX, y, z)) { // if it is in simulation box
          lattice_list->_lattices[z][y][x].id = 0x200;
        } else { // ghost area.
          lattice_list->_lattices[z][y][x].id = 0x100;
        }
      }
    }
  }

  //  establish ghost area by communicating for all sectors.
  mpi_types::setInterMPIType();
  GhostInitPacker init_packer{p_domain, lattice_list};
  comm::neiSendReceive(&init_packer, SimulationDomain::comm_sim_pro, mpi_types::_mpi_type_lattice_data,
                       p_domain->rank_id_neighbours);
  mpi_types::unsetInterMPIType();

  // test lattice in ghost area
  for (comm::_type_lattice_coord z = 0; z < p_domain->local_ghost_ext_lattice_region.z_high; z++) {
    for (comm::_type_lattice_coord y = 0; y < p_domain->local_ghost_ext_lattice_region.y_high; y++) {
      // due to BCC, x is doubled.
      for (comm::_type_lattice_coord x = 0; x < BCC_DBX * p_domain->local_ghost_ext_lattice_region.x_high; x++) {
        // all lattice id will be 200, not only simulation area.
        EXPECT_EQ(lattice_list->_lattices[z][y][x].id, 0x200);
      }
    }
  }
}
