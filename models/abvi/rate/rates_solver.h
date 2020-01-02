//
// Created by genshen on 2019-01-10.
//

#ifndef MISA_KMC_RATES_SOLVER_H
#define MISA_KMC_RATES_SOLVER_H

#include "../box.h"

/**
 * \brief interface of rates solver for different type of lattice.
 * \details The formula to calculate transition rate is show as following:
 * k=Ae^{ frac{ -E_a }{ K_bT } }, in which E_a = e0+ (e_after - e_before) / 2.
 * where e_after and e_before is the system energy after and before performing transition.
 */
class RatesSolver {
public:

    /**
     * \brief initialize solver with lattice lists.
     * \param lat_list reference of all lattice points
     *  (we need to known the status of neighbours when calculating rate).
     * \param v attempt frequency
     * \param T temperature in K unit.
     */
    explicit RatesSolver(LatticesList &lat_list, const double v, const double T);

    /**
     * \brief return the transition rate from source lattice specified by @param source_lattice
     * to its neighbour lattice specified by @param target_lattice.
     * \note api caller should exclude the case of dumbbell or vacancy type of target lattice.
     *
     * \param source_lattice reference of source lattice
     * \param target_lattice reference of target lattice
     * \param ghost_atom nature of atom exchanged with vacancy or moving in dumbbell.
     *     More detailed, it is the atom type exchanged with vacancy in vacancy transition,
     *     or atom type moving in dumbbell in dumbbell transition
     * \param _1nn_offset offset of target lattice .
     * \return the transition rate.
     */
    virtual const _type_rate rate(Lattice &source_lattice, Lattice &target_lattice,
                                  const LatticeTypes::lat_type ghost_atom,
                                  const _type_dir_id _1nn_offset);

protected:
    /**
     * \brief reference of all lattice points.
     */
    LatticesList &lattice_list;

    /**
     * \brief attempt frequency, unit: 1/s
     */
    const double attempt_freq = 0;

    /**
     * \brief system temperature.
     */
    const double temperature = 0;

    /**
     * \brief return e0 in formula: E_a = e0+ (e_after - e_before) / 2
     * \param ghost_atom nature of atom exchanged with vacancy or moving in dumbbell.
     * \return e0
     */
    virtual double e0(const LatticeTypes::lat_type ghost_atom) const = 0;

    /**
     * \brief calculate the difference of system energy after and before transition.
     * \param source_id local id of source lattice
     * \param target_id local id of target lattice
     * \param source_lattice ref of source lattice
     * \param target_lattice ref of target lattice
     * \param ghost_atom nature of atom exchanged with vacancy or moving in dumbbell.
     * \return the difference of system energy after and before transition.
     */
    virtual double deltaE(_type_lattice_id source_id, _type_lattice_id target_id,
                          Lattice &source_lattice, Lattice &target_lattice,
                          const LatticeTypes::lat_type ghost_atom) = 0;

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
};

#endif //MISA_KMC_RATES_SOLVER_H
