//
// Created by genshen on 2018-12-12.
//

#include "lattice_types.h"
#include <cstdlib>
#include <utils/random/random.h>

LatticeTypes::lat_type LatticeTypes::randomAtomsType(const lat_type source_type[], const unsigned int ratio[],
                                                     const unsigned int len, const unsigned int hit) {
  unsigned int rank_local = 0;
  for (unsigned int i = 0; i < len; i++) {
    rank_local += ratio[i];
    if (rank_local >= hit) {
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
