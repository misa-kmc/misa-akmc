//
// Created by genshen on 2019-01-19.
//

#ifndef MISA_KMC_ITL_RATES_SOLVER_H
#define MISA_KMC_ITL_RATES_SOLVER_H

#include "rates_solver.h"

/**
 * \brief calculating transition rate for vacancy transition.
 */
class ItlRatesSolver : public RatesSolver {
public:
  /**
   * \brief initialize itl rates solver with lattice list, vacancy list and itl list.
   * \param lat_list lattice list
   * \param va_list vacancy list
   * \param itl_list itl list
   * \param v attempt frequency
   * \param T temperature in K unit.
   */
  explicit ItlRatesSolver(LatticesList &lat_list, VacancyList &va_list, ItlList &itl_list, const double v,
                          const double T);

  /**
   * \brief get e0 in formula: E_a = e0+ (e_after - e_before) / 2
   * \param ghost_atom nature of atom exchanged with vacancy or moving in dumbbell.
   *     More detailed, it is the atom type exchanged with vacancy in vacancy transition,
   *     or atom type moving in dumbbell in dumbbell transition (this case in this class).
   *     \see base class for more details.
   *     It will equal to the transition atom type of source lattice in dumbbell transition.
   * \return e0
   */
  double e0(const LatticeTypes::lat_type ghost_atom) const override;

  /**
   * \brief calculate the difference of system energy after and before transition.
   * \param source_id local id of source lattice
   * \param target_id local id of target lattice
   * \param source_lattice ref of source lattice
   * \param target_lattice ref of target lattice
   * \param ghost_atom nature of atom exchanged with vacancy or moving in dumbbell.
   * \return the difference of system energy after and before transition.
   */
  double deltaE(_type_lattice_id source_id, _type_lattice_id target_id, Lattice &source_lattice,
                Lattice &target_lattice, const LatticeTypes::lat_type ghost_atom) override;

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

#endif // MISA_KMC_ITL_RATES_SOLVER_H
