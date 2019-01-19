//
// Created by genshen on 2019-01-19.
//

#ifndef MISA_KMC_ITL_RATES_SOLVER_H
#define MISA_KMC_ITL_RATES_SOLVER_H


#include "rates_solver.h"

class ItlRatesSolver : public RatesSolver {
public:
    ItlRatesSolver(Box &box);

    /**
     * \brief return the transition rate from lattice specified by @param source_lattice and @param x,y,z
     * to its neighbour specified by 1nn_id @param _1nn_id.
     * \param x,y,z the coordinate of source_lattice
     * \param source_lattice reference of source lattice
     * \param _1nn_id 1nn neighbour lattice id.
     * \return the transition rate.
     */
    _type_rate rate(const _type_lattice_coord x, const _type_lattice_coord y, const _type_lattice_coord z,
                    const Lattice &source_lattice, const _type_dir_id _1nn_id);
};


#endif //MISA_KMC_ITL_RATES_SOLVER_H
