//
// Created by genshen on 2018-12-12.
//

#include <cstdlib>
#include "lattice_types.h"

LatticeTypes::lat_type LatticeTypes::randomAtomsType(int ratio[], int len) {
    int ratio_total = 0;
    for (int i = 0; i < c; i++) {
        ratio_total += ratio[i];
    }
#ifdef DEV_MODE
    int rand_ = rand() % ratio_total;
#else
    int rand_ = rand() % ratio_total; // todo srank. Rand() has limited randomness; use C++ lib instead.
#endif

    int rank_local = 0;
    for (int i = 1; i <= c; i++) {
        rank_local += ratio[i];
        if (rand_ < rank_local) {
            return static_cast<lat_type>(i);
        }
    }
    return Fe;
}

LatticeTypes::lat_type LatticeTypes::combineToInter(lat_type atom_a, lat_type atom_b) {
    if (atom_a > atom_b) {
        lat_type temp = atom_a;
        atom_a = atom_b;
        atom_b = temp;
    }
    return static_cast<lat_type>((atom_a << big_endian_shift) | atom_b);
}
