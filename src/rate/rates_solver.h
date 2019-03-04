//
// Created by genshen on 2019-01-10.
//

#ifndef MISA_KMC_RATES_SOLVER_H
#define MISA_KMC_RATES_SOLVER_H

#include "box.h"

/**
 * \brief interface of rates solver for different type of lattice.
 */
class RatesSolver {
public:

    /**
     * \brief initialize solver with lattice lists.
     * \param lat_list reference of all lattice points
     *  (we need to known the status of neighbours when calculating rate).
     */
    explicit RatesSolver(LatticesList &lat_list);

    /**
     * \brief return the transition rate from source lattice specified by @param source_lattice
     * to its neighbour lattice specified by @param target_lattice.
     * \param source_lattice reference of source lattice
     * \param target_lattice reference of target lattice
     * \param trans_atom transition atom for dumbbell or vacancy.
     * \param _1nn_offset offset of target lattice .
     * \return the transition rate.
     */
    virtual _type_rate rate(Lattice &source_lattice, Lattice &target_lattice,
                            const LatticeTypes::lat_type trans_atom,
                            const _type_dir_id _1nn_offset) = 0;

protected:
    /**
     * \brief reference of all lattice points.
     */
    LatticesList &lattice_list;

    /**
     * \brief calculate the rate based on delta e.
     *
     * Arrhenius equation:
     * k=Ae^{ frac{ -E_a }{ K_bT } }
     * see https://en.wikipedia.org/wiki/Arrhenius_equation for more details.
     *
     * \param v attempt frequencies
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
