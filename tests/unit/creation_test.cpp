//
// Created by genshen on 2019/12/16.
//

#include <creation.h>
#include <gtest/gtest.h>
#include <lattice/period_lattice_list.h>
#include <utils/macros.h>
#include <utils/mpi_utils.h>

// @MPI
TEST(creation_setGlobalId_allreduce_test, creation_setGlobalId_test) {
  const int grid_size[3] = {2, 2, 2};
  const int64_t space[3] = {50, 63, 71};
  const double lattice_const = 0.86;
  const double cutoff_radius_factor = 1.1421;
  // init domain
  MPI_Comm new_comm;
  comm::mpi_process pro = comm::mpi_process{
      kiwi::mpiUtils::local_process.own_rank,
      kiwi::mpiUtils::local_process.all_ranks,
      kiwi::mpiUtils::local_process.comm,
  };
  comm::ColoredDomain *p_domain = comm::ColoredDomain::Builder()
                                      .setComm(pro, &new_comm)
                                      .setPhaseSpace(space)
                                      .setCutoffRadius(cutoff_radius_factor)
                                      .setLatticeConst(lattice_const)
                                      .build();

  PeriodLatticeList lattice_list(LatListMeta{static_cast<_type_box_size>(p_domain->sub_box_lattice_size[0]),
                                             static_cast<_type_box_size>(p_domain->sub_box_lattice_size[1]),
                                             static_cast<_type_box_size>(p_domain->sub_box_lattice_size[2]),
                                             static_cast<_type_box_size>(p_domain->lattice_size_ghost[0]),
                                             static_cast<_type_box_size>(p_domain->lattice_size_ghost[1]),
                                             static_cast<_type_box_size>(p_domain->lattice_size_ghost[2])});
  creation::setGlobalId(&lattice_list, p_domain->local_sub_box_lattice_region, p_domain->sub_box_lattice_region,
                        {p_domain->phase_space[0], p_domain->phase_space[1], p_domain->phase_space[2]});

  // sum ids of lattices in simulation box on current process
  comm::Region<comm::_type_lattice_coord> lbr = p_domain->local_sub_box_lattice_region;

  _type_lattice_id id_local_sum = 0;
  _type_lattice_id id_sum = 0;
  for (comm::_type_lattice_coord z = lbr.x_low; z < lbr.z_high; z++) {
    for (comm::_type_lattice_coord y = lbr.y_low; y < lbr.y_high; y++) {
      for (comm::_type_lattice_coord x = BCC_DBX * lbr.x_low; x < BCC_DBX * lbr.x_high; x++) {
        //                id_local_sum += lattice_list.getId(x,y,z);
        id_local_sum += lattice_list.getLat(x, y, z).getId();
      }
    }
  }
  MPI_Allreduce(&id_local_sum, &id_sum, 1, MPI_UNSIGNED_LONG, MPI_SUM, MPI_COMM_WORLD);
  // test sum of all global ids
  EXPECT_EQ(id_sum, BCC_DBX * space[0] * space[1] * space[2] * (BCC_DBX * space[0] * space[1] * space[2] - 1) / 2);

  delete p_domain;
}
