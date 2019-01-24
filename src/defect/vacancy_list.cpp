//
// Created by genshen on 2018-12-18.
//

#include "vacancy_list.h"

void Vacancy::beforeRatesUpdate(Lattice *list_1nn[LatticesList::MAX_1NN], _type_neighbour_status status_1nn) {
    // zero rates array
    Defect::beforeRatesUpdate(list_1nn, status_1nn);
    // set available transition dir.
    avail_trans_dir = availTranDirs(status_1nn, list_1nn);
}

void Vacancy::updateRates(Lattice &lattice, Lattice *list_1nn[LatticesList::MAX_1NN],
                          _type_neighbour_status status_1nn,
                          rateCallback callback) {
    for (unsigned char b = 0; b < LatticesList::MAX_NEI_BITS; b++) {
        if ((avail_trans_dir >> b) & 1) { // the neighbour lattice is available
            rates[b] = callback(list_1nn[b], lattice.type._type, b);
        }
    }
}
