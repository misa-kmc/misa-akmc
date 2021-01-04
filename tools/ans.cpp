//
// Created by genshen on 2020/1/27.
//

#include "ans.h"
#include <comm/domain/region.hpp>
#include <iostream>

void ans::analysis(LatticesList *lats_list) {
  comm::Region<_type_lattice_coord> region(lats_list->meta.ghost_x, lats_list->meta.ghost_y, lats_list->meta.ghost_z,
                                           lats_list->meta.ghost_x + lats_list->meta.box_x,
                                           lats_list->meta.ghost_y + lats_list->meta.box_y,
                                           lats_list->meta.ghost_z + lats_list->meta.box_z);

  Lattice *_1nn_list[LatticesList::MAX_1NN] = {nullptr};
  size_t single_cu = 0;
  for (_type_lattice_coord z = region.z_low; z < region.z_high; z++) {
    for (_type_lattice_coord y = region.y_low; y < region.y_high; y++) {
      for (_type_lattice_coord x = region.x_low; x < region.x_high; x++) {
        Lattice lattice = lats_list->getLat(x, y, z);
        if (lattice.type._type == LatticeTypes::Cu) {
          lats_list->get1nn(x, y, z, _1nn_list);
          const _type_neighbour_status nei_status = lats_list->get1nnBoundaryStatus(x, y, z);
          bool has_neighbor_cu = false;
          // travel its 1nn  neighbor
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
        }
      }
    }
  }
  std::cout << single_cu << std::endl;
}
