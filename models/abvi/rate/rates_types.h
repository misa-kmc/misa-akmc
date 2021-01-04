//
// Created by genshen on 2019-01-19.
//

#ifndef MISA_KMC_RATES_TYPES_H
#define MISA_KMC_RATES_TYPES_H

#include "../orientation.h"
#include "type_define.h"

/**
 * \brief Boltzmann constant in unit eV/k.
 *  unit: eV/K, also equals to 1.3806485279e-23 with unit J/K
 *  Boltzmann Constant in LAKIMOCA is 8.625E-5
 */
const double BoltzmannConstant = 8.617330350E-5;

/**
 * \brief callback interface to get transition rate of inter lattice or vacancy lattice.
 * ghost_atom is the atom exchanged with vacancy or moving in dumbbell.
 */
typedef std::function<_type_rate(Lattice *lat_nei, const LatticeTypes::lat_type ghost_atom,
                                 const _type_dir_id _1nn_offset)>
    rateCallback;
// typedef _type_rate(*rateCallback)(_type_dir_id _1nn_offset); // A lambda can only be converted to a function pointer
// if it does not capture

#endif // MISA_KMC_RATES_TYPES_H
