//
// Created by genshen on 2019-01-24.
//

#include <cassert>

#include "../env.h"
#include "bonds/bonds_counter.h"
#include "vacancy_rates_solver.h"

VacRatesSolver::VacRatesSolver(LatticesList &lat_list, const double v, const double T) : RatesSolver(lat_list, v, T) {}

double VacRatesSolver::e0(const LatticeTypes::lat_type ghost_atom) const {
  switch (ghost_atom) {
  case LatticeTypes::Fe:
    return 0.62;
  case LatticeTypes::Cu:
    return 0.54;
  case LatticeTypes::Ni:
    return 0.68;
  case LatticeTypes::Mn:
    return 1.03;
  default:
    return 0;
  }
}

double VacRatesSolver::deltaE(_type_lattice_id source_id, _type_lattice_id target_id, Lattice &source_lattice,
                              Lattice &target_lattice, const LatticeTypes::lat_type ghost_atom) {
#ifdef KMC_DEBUG_MODE
  {
    const bool debug_bool_1 = target_lattice.type.isDumbbell(); // always be false.
    assert(!debug_bool_1);
    const bool debug_bool_2 = source_lattice.type.isVacancy(); // always be true.
    assert(debug_bool_2);
    // in vacancy transition, ghost atom equals to the type of target lattice(target lattice will be a single atom)
    assert(target_lattice.type._type == ghost_atom);
  };
#endif

#ifdef EAM_POT_ENABLED
  return 0; // todo eam
#else
  // calculate system energy before transition.
  double e_before = 0;
  {
    // bonds energy of src lattice contributed by its 1nn/2nn neighbour lattice.
    bonds::_type_pair_ia e_src = bonds::BondsCounter::count(&lattice_list, source_id, source_lattice.type);
    // bonds energy of des lattice contributed by its 1nn/2nn neighbour lattice(it is an atom).
    bonds::_type_pair_ia e_des = bonds::BondsCounter::count(&lattice_list, target_id, target_lattice.type);
    e_before = e_src + e_des;
  }

  // exchange atoms of vacancy and neighbour lattice atom/vacancy.
  // note: ghost_atom equals to origin target_lattice.type
  source_lattice.type = LatticeTypes{ghost_atom};
  target_lattice.type = LatticeTypes{LatticeTypes::V};

  // calculate system energy after transition.
  double e_after = 0;
  {
    bonds::_type_pair_ia e_src = bonds::BondsCounter::count(&lattice_list, source_id, source_lattice.type);
    bonds::_type_pair_ia e_des = bonds::BondsCounter::count(&lattice_list, target_id, target_lattice.type);
    e_after = e_src + e_des;
  }

  // exchange atoms back.
  source_lattice.type = LatticeTypes{LatticeTypes::V};
  target_lattice.type = LatticeTypes{ghost_atom};

  return e_after - e_before;
#endif
  return 0;
}
