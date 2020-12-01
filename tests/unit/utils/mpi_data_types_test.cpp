//
// Created by genshen on 2019/10/26.
//

#include <gtest/gtest.h>
#include <lattice/lattice.h>
#include <mpi.h>
#include <utils/mpi_types.h>
#include <utils/mpi_utils.h>

TEST(set_set_mpi_data_type_attice_test, set_mpi_data_type_test) {
  MPI_Datatype lat_mpi_data_type;
  mpi_types::setMPI_DataTypeLattice(&lat_mpi_data_type);

  const kiwi::RID target_rank =
      (kiwi::mpiUtils::global_process.own_rank + 1) % kiwi::mpiUtils::global_process.all_ranks;
  const kiwi::RID source_rank =
      (kiwi::mpiUtils::global_process.own_rank + kiwi::mpiUtils::global_process.all_ranks - 1) %
      kiwi::mpiUtils::global_process.all_ranks; // note: -1 % 5 = -1, not 4
  Lattice lattice_send[5];
  for (int i = 0; i < 5; i++) {
    lattice_send[i].type = LatticeTypes{LatticeTypes::FeCu};
    lattice_send[i].id = 1000 + i * target_rank;
  }
  Lattice lattice_recv[5];

  MPI_Status status;
  MPI_Sendrecv(lattice_send, 5, lat_mpi_data_type, target_rank, 0x200, lattice_recv, 5, lat_mpi_data_type, source_rank,
               0x200, MPI_COMM_WORLD, &status);

  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(lattice_recv[i].type._type, LatticeTypes::FeCu);
    EXPECT_EQ(lattice_recv[i].id, 1000 + i * kiwi::mpiUtils::global_process.own_rank);
  }

  MPI_Type_free(&lat_mpi_data_type);
}
