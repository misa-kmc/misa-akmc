//
// Created by genshen on 2019-01-19.
//

#ifndef MISA_KMC_ITL_RATES_SOLVER_H
#define MISA_KMC_ITL_RATES_SOLVER_H


#include "rates_solver.h"

class ItlRatesSolver : public RatesSolver {
public:
    /**
     * \brief initialize itl rates solver with lattice list, vacancy list and itl list.
     * \param lat_list lattice list
     * \param va_list vacancy list
     * \param itl_list itl list
     * \param v
     * \param T
     */
    explicit ItlRatesSolver(LatticesList &lat_list,
                            VacancyList &va_list,
                            ItlList &itl_list,
                            double v, const double T);

    /**
     * // todo tests
     * \brief return the transition rate from source lattice specified by @param source_lattice
     * to its neighbour lattice specified by @param target_lattice.
     * \param source_lattice reference of source lattice, witch is a dumbbell.
     * \param target_lattice reference of target lattice, which is an atom.
     * \param trans_atom transition atom for dumbbell.
     * \param _1nn_offset offset of target lattice .
     * \return the transition rate.
     */
    _type_rate rate(Lattice &source_lattice, Lattice &target_lattice,
                    const LatticeTypes::lat_type trans_atom,
                    const _type_dir_id _1nn_offset) override;

private:
    /*!
    * \brief reference of list of vacancy indexed by lattice id.
    */
    VacancyList &va_list;

    /*!
     * \brief reference of list of interval lattice (dumbbell) indexed by lattice id.
     */
    ItlList &itl_list;
};


#endif //MISA_KMC_ITL_RATES_SOLVER_H
