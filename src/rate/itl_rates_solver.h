//
// Created by genshen on 2019-01-19.
//

#ifndef MISA_KMC_ITL_RATES_SOLVER_H
#define MISA_KMC_ITL_RATES_SOLVER_H


#include "rates_solver.h"

class ItlRatesSolver : public RatesSolver {
public:
    explicit ItlRatesSolver(Box &box, const double v, const double T);

    /**
     * \brief return the transition rate from source lattice specified by @param source_lattice
     * to its neighbour lattice specified by @param target_lattice.
     * \param source_lattice reference of source lattice
     * \param target_lattice reference of target lattice
     * \param trans_atom transition atom for dumbbell or just vacancy.
     * \param _1nn_offset offset of target lattice .
     * \return the transition rate.
     */
    _type_rate rate(const Lattice &source_lattice, const Lattice &target_lattice,
                    const LatticeTypes::lat_type trans_atom,
                    const _type_dir_id _1nn_offset) override;
};


#endif //MISA_KMC_ITL_RATES_SOLVER_H
