//
// Created by genshen on 2019-01-19.
//

#include <cassert>

#include "../env.h"
#include "../rate/bonds/bonds_counter.h"
#include "bonds/energy_dumbbell.h"
#include "itl_rates_solver.h"

ItlRatesSolver::ItlRatesSolver(LatticesList &lat_list, VacancyList &va_list, ItlList &itl_list, const double v,
                               const double T)
    : RatesSolver(lat_list, v, T), va_list(va_list), itl_list(itl_list) {}

double ItlRatesSolver::e0(const LatticeTypes::lat_type ghost_atom) const {

  switch (ghost_atom) {
  case LatticeTypes::Fe:
    return 0.32;
  case LatticeTypes::Cu:
    return 0.36;
  case LatticeTypes::Ni:
    return 0.45;
  case LatticeTypes::Mn:
    return 0.31;
  default:
    return 0;
  }
}

double ItlRatesSolver::deltaE(_type_lattice_id source_id, _type_lattice_id target_id, Lattice &source_lattice,
                              Lattice &target_lattice, const LatticeTypes::lat_type ghost_atom) {

#ifdef KMC_DEBUG_MODE
  {
    const bool debug_bool_1 = source_lattice.type.isDumbbell();
    assert(debug_bool_1);
    const bool debug_bool_2 = target_lattice.type.isAtom();
    assert(debug_bool_2);
  };
#endif

#ifdef EAM_POT_ENABLED
  return 0; // todo eam
#else
  // calculate system energy before transition.
  double e_before = 0;
  {
    // bonds energy of src lattice contributed by its 1nn/2nn neighbour lattice.
    bonds::_type_pair_ia e_src = bonds::BondsCounter::count(&lattice_list, source_id, LatticeTypes{ghost_atom});
    // bonds energy of des lattice contributed by its 1nn/2nn neighbour lattice(it is an atom).
    bonds::_type_pair_ia e_des = bonds::BondsCounter::count(&lattice_list, target_id, target_lattice.type);
    // the count of dumbbells does not change, so we does not count the term: sum_itl *Ef110.
    double e_dumbbell = bond::Edumb(lattice_list, itl_list);
    e_before = e_src + e_des + e_dumbbell;
  }

  // exchange atoms of jump atom and neighbour lattice atom.
  const LatticeTypes cached_source_type = source_lattice.type;
  const LatticeTypes cached_target_type = target_lattice.type;
  source_lattice.type._type = source_lattice.type.diff(LatticeTypes{ghost_atom});
  target_lattice.type._type = target_lattice.type.combineToInter(ghost_atom);
  // todo update orientation and dumbbell list?

  double e_after = 0;
  {
    bonds::_type_pair_ia e_src = bonds::BondsCounter::count(&lattice_list, source_id, source_lattice.type);
    bonds::_type_pair_ia e_des = bonds::BondsCounter::count(&lattice_list, target_id, LatticeTypes{ghost_atom});
    // the count of dumbbells does not change, so we does not count the term: sum_itl *Ef110.
    double e_dumbbell = bond::Edumb(lattice_list, itl_list);
    e_after = e_src + e_des + e_dumbbell;
  }

  // exchange atoms back.
  source_lattice.type = cached_source_type;
  target_lattice.type = cached_target_type;

  return e_after - e_before;
#endif
}
