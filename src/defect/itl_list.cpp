//
// Created by genshen on 2018-12-12.
//

#include "itl_list.h"

_type_dirs_status Itl::availTranDirs(_type_neighbour_status nei_status, Lattice *_1nn_lats[8]) {
    _type_dirs_status dir_status = direction.dir.availableTransDirs();
    // search all neighbour lattices, if the neighbour lattice is a destination that the source lattice can jump to
    // (it is available and it is atom),
    // then set the destination as available transition direction.
    _type_dirs_status atom_nei_status = 0;
    for (int b = 0; b < 8; b++) {
        // the neighbour lattice is available.
        // and the neighbour lattice is atom
        if (((nei_status >> b) & 1) && _1nn_lats[b]->type.isAtom()) { // can trans
            atom_nei_status |= 1 << b;
        }
    }
    return (dir_status & atom_nei_status);
}

void Itl::beforeRatesUpdate(Lattice *list_1nn[8], _type_neighbour_status status_1nn) {
    // compute and set "which neighbour lattices can jump to"
    avail_trans_dir = availTranDirs(status_1nn, list_1nn);
}

void Itl::updateRates(Lattice *list_1nn[8], _type_neighbour_status status_1nn) {
    _type_dirs_status trans_dirs = direction.dir.availableTransDirs();
    // search all neighbour lattices, if the neighbour lattice is a destination that the source lattice can jump to,
    // then calculate the transition rate from source lattice to the neighbour lattice.
    for (unsigned char b = 0; b < 8; b++) {
        if ((avail_trans_dir >> b) & 1) { // the neighbour lattice is available, and can trans.
            // the neighbour lattice is list_1nn[b]
            Lattice *lat_nei = list_1nn[b];
            // calculate the rate from itl_ref to lat_nei.
//            double Ea = deltaE();
            double rate = 0; // (v, T, Ea);  // todo compute rate
            rates[ratesIndex(b, trans_dirs, false)] = rate;
        }
    }
}

int Itl::ratesIndex(_type_dir_id _1nn_id, _type_dirs_status trans_dirs, bool up) {
    _type_dirs_status dir_status = trans_dirs & ~(static_cast<_type_dirs_status>(0xFF) << _1nn_id);
    static _type_dirs_status d1 = 0x5555, d2 = 0x3333, d3 = 0x0f0f;
    dir_status = (dir_status & d1) + ((dir_status >> 1) & d1);
    dir_status = (dir_status & d2) + ((dir_status >> 2) & d2);
    dir_status = (dir_status & d3) + ((dir_status >> 4) & d3);
    return up ? 2 * dir_status + 1 : 2 * dir_status;
}

Itl ItlList::getItlnum(_type_lattice_id id) {
    return mp[id];
}
