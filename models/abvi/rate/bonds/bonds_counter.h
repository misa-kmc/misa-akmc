//
// Created by genshen on 2019-01-20.
//

#ifndef MISA_KMC_BONDS_COUNTER_H
#define MISA_KMC_BONDS_COUNTER_H

#include "lattice/lattice_types.h"
#include "lattice/lattices_list.h"
#include "pair_bond.hpp"
#include "type_define.h"

namespace bonds {
  typedef double _type_pair_ia; // type of pair interactions
  /**
   * \brief count bonds of 1nn and 2nn neighbour lattices.
   *
   * \see {Vincent, E., C. S. Becquart, and C. Domain. \
   * "Solute interaction with point defects in α Fe during thermal ageing: A combined ab initio and atomic kinetic Monte
   * Carlo approach."  \ Journal of nuclear materials 351.1-3 (2006): 88-99} and table 7 for more details.
   */
  class BondsCounter {
  public:
    /**
     * \brief calculate the bonds energy contribution of the atom(specified by \param src_atom_type) in lattice
     * specified by \param source_id
     * \note We do not consider the contribution of dumbbell to this \param src_atom_type atom, which is considered in
     * EDumb(). Only vacancy and single atom contributions are considered.
     *
     * \param lat_list lattices list, we can get 1nn/2nn neighbour lattices from this list
     * \param source_id the id of source lattice
     * \param src_atom_type the atom type in source lattice(vacancy/single atom/inter's first/inter's second)
     * \return the bonds energy in 1nn and 2nn neighbour lattices of the atom(\param src_atom_type) in source lattice
     */
    static _type_pair_ia count(LatticesList *lat_list, _type_lattice_id source_id, LatticeTypes src_atom_type);

  protected:
    static const std::map<PairBond::bond_type, _type_pair_ia> _1nn_bonds;
    static const std::map<PairBond::bond_type, _type_pair_ia> _2nn_bonds;
  };
} // namespace bonds

#endif // MISA_KMC_BONDS_COUNTER_H
