//
// Created by genshen on 2020/1/27.
//

#include "ans.h"
#include <comm/domain/region.hpp>
#include <iostream>
#include <utils/mpi_utils.h>

void ans::analysis_result::reduce_to_rank(const int rank) {
  constexpr int reduce_length = 3;
  _type_count count_send[reduce_length] = {isolated_solutes, vacancies, lattice_num};
  _type_count count_recv[reduce_length] = {0, 0, 0};

  MPI_Reduce(count_send, count_recv, reduce_length, mpi_datatype, MPI_SUM, rank, kiwi::mpiUtils::global_process.comm);

  isolated_solutes = count_recv[0];
  vacancies = count_recv[1];
  lattice_num = count_recv[2];
}

ans::analysis_result ans::analysis(LatticesList *lats_list) {
  comm::Region<_type_lattice_coord> region(lats_list->meta.ghost_x, lats_list->meta.ghost_y, lats_list->meta.ghost_z,
                                           lats_list->meta.ghost_x + lats_list->meta.box_x,
                                           lats_list->meta.ghost_y + lats_list->meta.box_y,
                                           lats_list->meta.ghost_z + lats_list->meta.box_z);

  Lattice *_1nn_list[LatticesList::MAX_1NN] = {nullptr};
  size_t single_cu = 0;
  size_t v_count = 0;
  for (_type_lattice_coord z = region.z_low; z < region.z_high; z++) {
    for (_type_lattice_coord y = region.y_low; y < region.y_high; y++) {
      for (_type_lattice_coord x = region.x_low; x < region.x_high; x++) {
        Lattice lattice = lats_list->getLat(x, y, z);
        if (lattice.type._type == LatticeTypes::Cu) {
          lats_list->get1nn(x, y, z, _1nn_list);
          const _type_neighbour_status nei_status = lats_list->get1nnBoundaryStatus(x, y, z);
          bool has_neighbor_cu = false;
          // ro travel its 1nn  neighbor
          for (int b = 0; b < LatticesList::MAX_NEI_BITS; b++) {
            // the neighbour lattice is available.
            // and the neighbour lattice is also Cu
            if (((nei_status >> b) & 1) && (_1nn_list[b]->type._type == LatticeTypes::Cu)) {
              has_neighbor_cu = true;
            }
          }
          if (!has_neighbor_cu) {
            single_cu++;
          }
        } else if (lattice.type._type == LatticeTypes::V) {
          v_count++;
        }
      }
    }
  }

  analysis_result result;
  result.isolated_solutes = single_cu;
  result.vacancies = v_count;
  result.lattice_num = lats_list->meta.getLocalLatticeNum();
  result.reduce_to_rank(MASTER_PROCESSOR);

  return result;
}
