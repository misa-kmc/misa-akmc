//
// Created by genshen on 2018-12-18.
//

#include "vacancy_list.h"


_type_neighbour_status Vacancy::availTranDirs(_type_neighbour_status nei_status, Lattice **_1nn_lats) {
    int lat_index = 0;
    avail_trans_dir = 0; // clean flags
    // search all neighbour lattices, if the neighbour lattice is a destination that the source lattice can jump to,
    // then set the destination as available transition direction.
    for (int b = 0; b < LatticesList::MAX_NEI_BITS; b++) {
        if ((nei_status >> b) & 1) { // the neighbour lattice is available.
            // the neighbour lattice is _1nn_lats[lat_index]
            if (_1nn_lats[lat_index]->type.isAtom()) { // can trans
                avail_trans_dir |= 1 << b;
            }
            lat_index++;
        }
    }
    // todo return value
}

void Vacancy::beforeRatesUpdate(Lattice **list_1nn, _type_neighbour_status status_1nn) {
    Defect::beforeRatesUpdate(list_1nn, status_1nn);
}

void Vacancy::updateRates(Lattice **list_1nn, _type_neighbour_status status_1nn, rateCallback callback) {
    // todo implementations
}
