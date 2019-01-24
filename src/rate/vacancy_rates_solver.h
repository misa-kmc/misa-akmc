//
// Created by genshen on 2019-01-24.
//

#ifndef MISA_KMC_VACANCY_RATES_SOLVER_H
#define MISA_KMC_VACANCY_RATES_SOLVER_H


#include "rates_solver.h"

class VacRatesSolver : public RatesSolver {
public:
    explicit VacRatesSolver(Box &box);

    /**
     * \brief return the transition rate from source lattice specified by @param source_lattice
     * to its neighbour lattice specified by @param target_lattice.
     * \param source_lattice reference of source lattice whose type is vacancy.
     * \param target_lattice reference of target lattice whose type can be single atom or vacancy.
     * \param trans_atom type of transition lattice, always be vacancy in this case.
     * \param _1nn_offset offset of target lattice.
     * \return the transition rate.
     */
    _type_rate rate(Lattice &source_lattice,
                    Lattice &target_lattice,
                    const LatticeTypes::lat_type trans_atom,
                    const _type_dir_id _1nn_offset) override;
};


#endif //MISA_KMC_VACANCY_RATES_SOLVER_H