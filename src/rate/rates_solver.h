//
// Created by genshen on 2019-01-10.
//

#ifndef MISA_KMC_RATES_SOLVER_H
#define MISA_KMC_RATES_SOLVER_H

#include "box.h"

class RatesSolver {
public:
    _type_rate rate_sum;

    explicit RatesSolver(Box &box);

    /**
     * \brief return the transition rate from lattice specified by @param source_lattice and @param x,y,z
     * to its neighbour specified by 1nn_id @param _1nn_id.
     * \param x,y,z the coordinate of source_lattice
     * \param source_lattice reference of source lattice
     * \param _1nn_id 1nn neighbour lattice id.
     * \return the transition rate.
     */
    virtual _type_rate rate(const _type_lattice_coord x, const _type_lattice_coord y, const _type_lattice_coord z,
                            const Lattice &source_lattice, const _type_dir_id _1nn_id) = 0;

protected:
    const Box &box;

    /**
     * \brief calculate the rate based on delta e.
     *
     * Arrhenius equation:
     * k=Ae^{ frac{ -E_a }{ K_bT } }
     * see https://en.wikipedia.org/wiki/Arrhenius_equation for more details.
     *
     * \param v attempt rate
     * \param T temperature
     * \param Ea activation energy for the reaction (in the same units as kB*T)
     * \return the transition rate
     *
     */
    double arrhenius(const double v, const double T, const double delta_e);

    /**
     * return delta E
     */
    double activeEnergy();

};

#endif //MISA_KMC_RATES_SOLVER_H
