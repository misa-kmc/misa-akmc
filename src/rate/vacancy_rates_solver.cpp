//
// Created by genshen on 2019-01-24.
//

#include <cassert>

#include "env.h"
#include "vacancy_rates_solver.h"
#include "rate/bonds/bonds_counter.h"

VacRatesSolver::VacRatesSolver(LatticesList &lat_list) : RatesSolver(lat_list) {}

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

double VacRatesSolver::deltaE(Lattice &source_lattice, Lattice &target_lattice,
                              const LatticeTypes::lat_type trans_atom) {
#ifdef DEBUG_MODE
    {
        const bool debug_bool_1 = target_lattice.type.isDumbbell(); // always be false.
        assert(!debug_bool_1);
        const bool debug_bool_2 = source_lattice.type.isVacancy(); // always be true.
        assert(debug_bool_2);
    };
#endif

#ifdef EAM_POT_ENABLED
    return 0; // todo eam
#else
    //calculate system energy before transition.
    double e_before = 0;
    {
        // bonds energy of src lattice (trans_atom is always vacancy) contributed by its 1nn/2nn neighbour lattice.
        bonds::_type_pair_ia e_src = bonds::BondsCounter::count(&lattice_list,
                                                                source_lattice.getId(),
                                                                source_lattice.type);
        // bonds energy of des lattice contributed by its 1nn/2nn neighbour lattice(it is an atom).
        bonds::_type_pair_ia e_des = bonds::BondsCounter::count(&lattice_list,
                                                                target_lattice.getId(),
                                                                target_lattice.type);
        e_before = e_src + e_des;
    }

    // exchange atoms of vacancy and neighbour lattice atom/vacancy.
    source_lattice.type = target_lattice.type;
    target_lattice.type = LatticeTypes{trans_atom};

    // calculate system energy after transition.
    double e_after = 0;
    {
        bonds::_type_pair_ia e_src = bonds::BondsCounter::count(&lattice_list,
                                                                source_lattice.getId(),
                                                                source_lattice.type);
        bonds::_type_pair_ia e_des = bonds::BondsCounter::count(&lattice_list,
                                                                target_lattice.getId(),
                                                                target_lattice.type);
        e_after = e_src + e_des;
    }

    // exchange atoms back.
    target_lattice.type = source_lattice.type;
    source_lattice.type = LatticeTypes{trans_atom};

    return e_after - e_before;
#endif
    return 0;
}
