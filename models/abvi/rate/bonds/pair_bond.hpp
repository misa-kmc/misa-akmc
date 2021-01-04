//
// Created by genshen on 2019-01-20.
//

#ifndef MISA_KMC_PAIR_BOND_H
#define MISA_KMC_PAIR_BOND_H

#include "lattice/lattice_types.h"

namespace bonds {
  struct PairBond {
    /**
     * \brief enum of pairs among {V,Fe,Cu,Ni,Mn}
     *
     */
    typedef enum {
      VV = 0x0000,
      VFe = 0x0001,
      VCu = 0x0002,
      VNi = 0x0004,
      VMn = 0x0008,
      FeFe = 0x0101,
      FeCu = 0x0102,
      FeNi = 0x0104,
      FeMn = 0x0108,
      CuCu = 0x0202,
      CuNi = 0x0204,
      CuMn = 0x0208,
      NiNi = 0x0404,
      NiMn = 0x0408,
      MnMn = 0x0808,
    } bond_type;
    const bond_type bond;

    const static uint16_t big_endian_shift = 8;

    explicit PairBond(bond_type b) : bond(b) {}

    /**
     * \brief combine two atom types into bond type.
     * The parameter types are LatticeTypes, instead of LatticeTypes::bond_type
     * \param atom_a type of atom A
     * \param atom_b type of atom B
     * \return the combined bond type.
     */
    static inline bond_type makeBond(LatticeTypes atom_a, LatticeTypes atom_b) {
      return makeBond(atom_a._type, atom_b._type);
    }

    /**
     * \brief  combine two atom types into bond type.
     * create an bond form \param atom_a and \param atom_b
     * for example  \param atom_a is Fe and \param atom_b is Cu, this function will return bond FeCu.
     * \param atom_a type of atom A
     * \param atom_b type of atom B
     * \return the combined bond type.
     */
    static bond_type makeBond(LatticeTypes::lat_type atom_a, LatticeTypes::lat_type atom_b) {
      if (atom_a > atom_b) {
        LatticeTypes::lat_type temp = atom_a;
        atom_a = atom_b;
        atom_b = temp;
      }
      return static_cast<bond_type>((atom_a << big_endian_shift) | atom_b);
    }
  };
} // namespace bonds

#endif // MISA_KMC_PAIR_BOND_H
