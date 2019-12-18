//
// Created by genshen on 2019-01-10.
//

#include "period_lattice_list.h"

PeriodLatticeList::PeriodLatticeList(const LatListMeta meta) : LatticesList(meta) {}

_type_neighbour_status PeriodLatticeList::get1nnStatus(_type_lattice_coord x,
                                                       _type_lattice_coord y,
                                                       _type_lattice_coord z) {
    return 0xFF;
}

_type_neighbour_status PeriodLatticeList::get2nnStatus(_type_lattice_coord x,
                                                       _type_lattice_coord y,
                                                       _type_lattice_coord z) {
    return 0xFF;
}

int PeriodLatticeList::get1nn(_type_lattice_coord x, _type_lattice_coord y,
                              _type_lattice_coord z, Lattice **_1nn_list) {
    if (x % 2 == 0) {
        _1nn_list[0] = &_lattices[z - 1][y - 1][x - 1];
        _1nn_list[1] = &_lattices[z + 0][y - 1][x - 1];
        _1nn_list[2] = &_lattices[z - 1][y + 0][x - 1];
        _1nn_list[3] = &_lattices[z + 0][y + 0][x - 1];
        _1nn_list[4] = &_lattices[z - 1][y - 1][x + 1];
        _1nn_list[5] = &_lattices[z + 0][y - 1][x + 1];
        _1nn_list[6] = &_lattices[z - 1][y + 0][x + 1];
        _1nn_list[7] = &_lattices[z + 0][y + 0][x + 1];
    } else {
        _1nn_list[0] = &_lattices[z + 0][y + 0][x - 1];
        _1nn_list[1] = &_lattices[z + 1][y + 0][x - 1];
        _1nn_list[2] = &_lattices[z + 0][y + 1][x - 1];
        _1nn_list[3] = &_lattices[z + 1][y + 1][x - 1];
        _1nn_list[4] = &_lattices[z + 0][y + 0][x + 1];
        _1nn_list[5] = &_lattices[z + 1][y + 0][x + 1];
        _1nn_list[6] = &_lattices[z + 0][y + 1][x + 1];
        _1nn_list[7] = &_lattices[z + 1][y + 1][x + 1];
    }
    return 8;
}

int PeriodLatticeList::get2nn(_type_lattice_coord x, _type_lattice_coord y,
                              _type_lattice_coord z, Lattice **_2nn_list) {
    _2nn_list[0] = &_lattices[z + 0][y + 0][x - 2];
    _2nn_list[1] = &_lattices[z + 0][y - 1][x + 0];
    _2nn_list[2] = &_lattices[z - 1][y + 0][x + 0];
    _2nn_list[3] = &_lattices[z + 1][y + 0][x + 0];
    _2nn_list[4] = &_lattices[z + 0][y + 1][x + 0];
    _2nn_list[5] = &_lattices[z + 0][y + 0][x + 2];
    return 6;
}
