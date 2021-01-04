//
// Created by genshen on 2019/12/30.
//

#include "../domain_test_fixtures.h"
#include <algorithms/selector.h>
#include <algorithms/sl/comm_dirs.h>
#include <algorithms/sl/sublattice.h>
#include <comm/preset/sector_forwarding_region.h>
#include <gtest/gtest.h>
#include <pack/sim_sync_packer.h>
#include <utils/macros.h>
#include <utils/mpi_types.h>

class TestPeriodLatticeList_SIM : public PeriodLatticeList {
public:
  explicit TestPeriodLatticeList_SIM(LatListMeta meta) : PeriodLatticeList(meta) {}

  FRIEND_TEST(SimSyncPackerTest, sim_sync_packer_test);
};

// test fixture class
class SimSyncPackerTest : public DomainSteUp<TestPeriodLatticeList_SIM> {};

// @MPI
TEST_F(SimSyncPackerTest, sim_sync_packer_test) {
  // set lattice id.
  for (comm::_type_lattice_coord z = 0; z < p_domain->local_ghost_ext_lattice_region.z_high; z++) {
    for (comm::_type_lattice_coord y = 0; y < p_domain->local_ghost_ext_lattice_region.y_high; y++) {
      // due to BCC, x is doubled.
      for (comm::_type_lattice_coord x = 0; x < BCC_DBX * p_domain->local_ghost_ext_lattice_region.x_high; x++) {
        lattice_list->_lattices[z][y][x].id = kiwi::mpiUtils::global_process.own_rank;
      }
    }
  }

  // prepare data
  SectorMeta sec_meta; // default is sector 0.
  const type_sector cur_sector = (*sec_meta.sector_itl);
  const int dims[comm::DIMENSION_SIZE] = {comm::DIM_X, comm::DIM_Y, comm::DIM_Z};
  SubLattice::type_comm_lat_regions send_regions; // send regions in each dimension
  SubLattice::type_comm_lat_regions recv_regions; // receive regions in each dimension.
  for (int d = 0; d < comm::DIMENSION_SIZE; d++) {
    send_regions[d] = comm::fwCommSectorRecvRegion(cur_sector.id, dims[d], p_domain->lattice_size_ghost,
                                                   p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);
    recv_regions[d] = comm::fwCommSectorSendRegion(cur_sector.id, dims[d], p_domain->lattice_size_ghost,
                                                   p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);
  }

  // pack data
  // call ssfdCommRecvDirs for send dirs and call ssfdCommSendDirs for receive
  // dirs.
  const std::array<unsigned int, comm::DIMENSION_SIZE> send_dirs = ssfdCommRecvDirs(cur_sector.id);
  const std::array<unsigned int, comm::DIMENSION_SIZE> recv_dirs = ssfdCommSendDirs(cur_sector.id);
  const std::array<unsigned int, comm::DIMENSION_SIZE> ranks_send = {
      static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_X][send_dirs[0]]),
      static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_Y][send_dirs[1]]),
      static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_Z][send_dirs[2]]),
  };
  const std::array<unsigned int, comm::DIMENSION_SIZE> ranks_recv = {
      static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_X][recv_dirs[0]]),
      static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_Y][recv_dirs[1]]),
      static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_Z][recv_dirs[2]]),
  };

  // do communication
  SimSyncPacker packer(lattice_list);
  mpi_types::setInterMPIType();

  // note here, communication order is from Z to Y, and X.
  comm::singleSideForwardComm<SimSyncPacker::pack_date_type, SimSyncPacker::pack_region_type, true>(
      &packer, SimulationDomain::comm_sim_pro, SimSyncPacker::getMPI_DataType(), send_regions, recv_regions, ranks_send,
      ranks_recv);
  mpi_types::unsetInterMPIType();

  // check received regions in section 0's simulation region communication.
  auto meta = lattice_list->meta;
  // - data from (x+1,y,z) process
  const auto r_x = comm::Region<comm::_type_lattice_coord>(
      meta.box_x, meta.ghost_y, meta.ghost_z, meta.ghost_x + meta.box_x, meta.ghost_y + p_domain->local_split_coord[1],
      meta.ghost_z + p_domain->local_split_coord[2]);
  // - data from (x,y+1,z) process
  const auto r_y = comm::Region<comm::_type_lattice_coord>(
      meta.ghost_x, meta.box_y, meta.ghost_z, meta.ghost_x + BCC_DBX * p_domain->local_split_coord[0],
      meta.ghost_y + meta.box_y, meta.ghost_z + p_domain->local_split_coord[2]);
  // - data from (x,y,z+1) process
  const auto r_z = comm::Region<comm::_type_lattice_coord>(
      meta.ghost_x, meta.ghost_y, meta.box_z, meta.ghost_x + BCC_DBX * p_domain->local_split_coord[0],
      meta.ghost_y + p_domain->local_split_coord[1], meta.ghost_z + meta.box_z);
  // - data from (x+1,y+1,z) process
  const auto r_xy =
      comm::Region<comm::_type_lattice_coord>(meta.box_x, meta.box_y, meta.ghost_z, meta.ghost_x + meta.box_x,
                                              meta.ghost_y + meta.box_y, meta.ghost_z + p_domain->local_split_coord[2]);
  // - data from (x+1,y,z+1) process
  const auto r_xz =
      comm::Region<comm::_type_lattice_coord>(meta.box_x, meta.ghost_y, meta.box_z, meta.ghost_x + meta.box_x,
                                              meta.ghost_y + p_domain->local_split_coord[1], meta.ghost_z + meta.box_z);
  // - data from (x,y+1,z+1) process
  const auto r_yz = comm::Region<comm::_type_lattice_coord>(meta.ghost_x, meta.box_y, meta.box_z,
                                                            meta.ghost_x + BCC_DBX * p_domain->local_split_coord[0],
                                                            meta.ghost_y + meta.box_y, meta.ghost_z + meta.box_z);
  // - data from (x+1,y+1,z+1) process
  const auto r_xyz =
      comm::Region<comm::_type_lattice_coord>(meta.box_x, meta.box_y, meta.box_z, meta.ghost_x + meta.box_x,
                                              meta.ghost_y + meta.box_y, meta.ghost_z + meta.box_z);
  const std::array<comm::Region<comm::_type_lattice_coord>, 7> regions = {r_x, r_y, r_z, r_xy, r_xz, r_yz, r_xyz};

  // get rand id of each neighbor ranks
  // note, periodic dimensions will be processed automatically.
  const int rank_coords_x[]{p_domain->grid_coord[0] + 1, p_domain->grid_coord[1], p_domain->grid_coord[2]};
  const int rank_coords_y[]{p_domain->grid_coord[0], p_domain->grid_coord[1] + 1, p_domain->grid_coord[2]};
  const int rank_coords_z[]{p_domain->grid_coord[0], p_domain->grid_coord[1], p_domain->grid_coord[2] + 1};
  const int rank_coords_xy[]{p_domain->grid_coord[0] + 1, p_domain->grid_coord[1] + 1, p_domain->grid_coord[2]};
  const int rank_coords_xz[]{p_domain->grid_coord[0] + 1, p_domain->grid_coord[1], p_domain->grid_coord[2] + 1};
  const int rank_coords_yz[]{p_domain->grid_coord[0], p_domain->grid_coord[1] + 1, p_domain->grid_coord[2] + 1};
  const int rank_coords_xyz[]{p_domain->grid_coord[0] + 1, p_domain->grid_coord[1] + 1, p_domain->grid_coord[2] + 1};
  kiwi::RID rank_x, rank_y, rank_z, rank_xy, rank_xz, rank_yz, rank_xyz;
  MPI_Cart_rank(kiwi::mpiUtils::global_process.comm, rank_coords_x,
                &rank_x); // rank (x+1,y,z)
  MPI_Cart_rank(kiwi::mpiUtils::global_process.comm, rank_coords_y,
                &rank_y); // rank (x,y+1,z)
  MPI_Cart_rank(kiwi::mpiUtils::global_process.comm, rank_coords_z,
                &rank_z); // rank (x,y+1,z)
  MPI_Cart_rank(kiwi::mpiUtils::global_process.comm, rank_coords_xy,
                &rank_xy); // rank (x+1,y+1,z)
  MPI_Cart_rank(kiwi::mpiUtils::global_process.comm, rank_coords_xz,
                &rank_xz); // rank (x+1,y,z+1)
  MPI_Cart_rank(kiwi::mpiUtils::global_process.comm, rank_coords_yz,
                &rank_yz); // rank (x,y+1,z+1)
  MPI_Cart_rank(kiwi::mpiUtils::global_process.comm, rank_coords_xyz,
                &rank_xyz); // rank (x+1,y+1,z+1)

  const std::array<kiwi::RID, 7> expected_ids{rank_x, rank_y, rank_z, rank_xy, rank_xz, rank_yz, rank_xyz};

  for (int i = 0; i < 7; i++) {
    auto _region = regions[i];
    for (int z = _region.z_low; z < _region.z_high; ++z) {
      for (int y = _region.y_low; y < _region.y_high; ++y) {
        for (int x = _region.x_low; x < _region.x_high; ++x) {
          EXPECT_EQ(lattice_list->getLat(x, y, z).id, expected_ids[i]);
        }
      }
    }
  }
}
