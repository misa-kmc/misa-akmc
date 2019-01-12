//
// Created by genshen on 2018-12-12.
//

#include "itl_list.h"

void Itl::updateAvailTranDir(_type_neighbour_status nei_status, Lattice *_1nn_lats[8]) {
    int lat_index = 0;
    avail_trans_dir = 0; // clean flags
    _type_dirs_status dir_statue = direction.dir.availableTransDirs();
    // search all neighbour lattices, if the neighbour lattice is a destination that the source lattice can jump to,
    // then set the destination as available transition direction.
    for (int b = 0; b < 8; b++) {
        if ((nei_status >> b) & 1) { // the neighbour lattice is available.
            // the neighbour lattice is _1nn_lats[lat_index]
            if (((dir_statue >> b) & 1) && _1nn_lats[lat_index]->type.isAtom()) { // can trans
                avail_trans_dir |= 1 << b;
            }
            lat_index++;
        }
    }
}

Itl ItlList::getItlnum(_type_lattice_id id) {
    return mp[id];
}
