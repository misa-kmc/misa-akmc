//
// Created by genshen on 2018-12-12.
//

#include <cstdlib>
#include <utils/random/random.h>
#include "lattice_types.h"

LatticeTypes::lat_type
LatticeTypes::randomAtomsType(const lat_type source_type[], const unsigned int ratio[], const int len) {
    int ratio_total = 0;
    for (int i = 0; i < len; i++) {
        ratio_total += ratio[i];
    }

    const int rand_ = r::rand32(0, static_cast<const uint32_t>(ratio_total));

    int rank_local = 0;
    for (int i = 0; i < len; i++) {
        rank_local += ratio[i];
        if (rank_local >= rand_) {
            return static_cast<lat_type>(source_type[i]);
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
    return static_cast<lat_type>((atom_a << high_endian_shift) | atom_b);
}

LatticeTypes::lat_type LatticeTypes::diff(const LatticeTypes A, const LatticeTypes B) const {
    if (A.getLowEnd() == B._type) {
        return A.getHighEnd();
    }
    if (A.getHighEnd() == B._type) {
        return A.getLowEnd();
    }
    return A._type;
}

LatticeTypes::lat_type LatticeTypes::diff(const LatticeTypes B) const {
    if (getLowEnd() == B._type) {
        return getHighEnd();
    }
    if (getHighEnd() == B._type) {
        return getLowEnd();
    }
    return _type;
}
