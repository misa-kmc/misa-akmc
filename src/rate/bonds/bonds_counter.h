//
// Created by genshen on 2019-01-20.
//

#ifndef MISA_KMC_BONDS_COUNTER_H
#define MISA_KMC_BONDS_COUNTER_H

#include "lattice/lattices_list.h"
#include "lattice/lattice_types.h"
#include "type_define.h"
#include "pair_bond.hpp"

typedef double _type_pair_ia; // type of pair interactions
/**
 * \brief count bonds of 1nn and 2nn neighbour lattices.
 *
 * \see {Vincent, E., C. S. Becquart, and C. Domain. \
 * "Solute interaction with point defects in α Fe during thermal ageing: A combined ab initio and atomic kinetic Monte Carlo approach."  \
 * Journal of nuclear materials 351.1-3 (2006): 88-99} and table 7 for more details.
 */
class BondsCounter {
    static const std::map<PairBond::bond_type, _type_pair_ia> _1nn_bonds;
    static const std::map<PairBond::bond_type, _type_pair_ia> _2nn_bonds;

};


#endif //MISA_KMC_BONDS_COUNTER_H
