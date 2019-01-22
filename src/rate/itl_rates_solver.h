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

    // todo tests
    double Edumb();

    /** // todo tests
     * \brief calculate compsol of dumbbell of FeX type (X can be Cu,Ni,Mn)
     * \param id id of lattice
     * \param type type of dumbbell lattice
     * \param itl the inter/dumbbell instance.
     * \param _1nns the 1nn neighbour lattices list.
     * \param _2nns the 2nn neighbour lattices list.
     * \param _1nn_status lattice status of 1nn neighbour lattices
     * \param _2nn_status lattice status of 2nn neighbour lattices
     * \return
     */
    double FeX_comp(const _type_lattice_id id,
                    const LatticeTypes type, const Itl &itl,
                    Lattice *_1nns[LatticesList::MAX_1NN],
                    Lattice *_2nns[LatticesList::MAX_2NN],
                    const _type_neighbour_status _1nn_status,
                    const _type_neighbour_status _2nn_status);

};


#endif //MISA_KMC_ITL_RATES_SOLVER_H
