//
// Created by genshen on 2019-01-24.
//

#ifndef MISA_KMC_VACANCY_RATES_SOLVER_H
#define MISA_KMC_VACANCY_RATES_SOLVER_H


#include "rates_solver.h"

/**
 * \brief solve the transition rate from a vacancy to its neighbour whose type is a single atom.
 * (in this model, neighbour type of dumbbell and vacancy is not allowed)
 * \note api caller should exclude the case of dumbbell and vacancy type of neighbour.
 */
class VacRatesSolver : public RatesSolver {
public:
    /**
     * \brief \see base class for more details.
     * \param lat_list lattice list.
     * \param v attempt frequency.
     * \param T temperature in K unit.
     */
    explicit VacRatesSolver(LatticesList &lat_list, const double v, const double T);

    /**
     * \brief get e0 in formula: E_a = e0+ (e_after - e_before) / 2
     * \param ghost_atom atom type exchanged with vacancy in vacancy transition (this case in this class),
     *     or atom type moving in dumbbell in dumbbell transition.
     *     \see base class for more details.
     *     It will equal to the type of target lattice(target lattice will be a single atom) in vacancy transition.
     * \return e0
     */
    double e0(const LatticeTypes::lat_type ghost_atom) const override;

    /**
     * \brief calculate the difference of system energy after and before transition.
     * \param source_id local id of source lattice
     * \param target_id local id of target lattice
     * \param source_lattice ref of source lattice
     * \param target_lattice ref of target lattice
     * \param ghost_atom the type of transition atom/vacancy
     * \return the difference of system energy after and before transition.
     */
    double deltaE(_type_lattice_id source_id, _type_lattice_id target_id,
                  Lattice &source_lattice, Lattice &target_lattice,
                  const LatticeTypes::lat_type ghost_atom) override;
};


#endif //MISA_KMC_VACANCY_RATES_SOLVER_H
