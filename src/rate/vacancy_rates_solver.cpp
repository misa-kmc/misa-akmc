//
// Created by genshen on 2019-01-24.
//

#include "vacancy_rates_solver.h"
#include "rate/bonds/bonds_counter.h"

VacRatesSolver::VacRatesSolver(Box &box) : RatesSolver(box) {

}

_type_rate VacRatesSolver::rate(Lattice &source_lattice,
                                Lattice &target_lattice,
                                const LatticeTypes::lat_type trans_atom,
                                const _type_dir_id _1nn_offset) {
#ifdef DEBUG_MODE
    {
        const bool debug_bool = target_lattice.type.isDumbbell(); // always be false.
        assert(!debug_bool);
    };
#endif

#ifdef EAM_POT_ENABLED
    return 0; // todo eam
#else
    double e0 = 0;
    switch (trans_atom) {
        case LatticeTypes::Fe:
            e0 = 0.62;
            break;
        case LatticeTypes::Cu:
            e0 = 0.54;
            break;
        case LatticeTypes::Ni:
            e0 = 0.68;
            break;
        case LatticeTypes::Mn:
            e0 = 1.03;
            break;
        default:
            e0 = 0;
            break;
    }

    //calculate system energy before transition.
    double e_before = 0;
    {
        // bonds energy of src lattice (trans_atom is always vacancy) contributed by its 1nn/2nn neighbour lattice.
        _type_pair_ia e_src = BondsCounter::count(box.lattice_list, source_lattice.getId(), source_lattice.type);
        // bonds energy of des lattice contributed by its 1nn/2nn neighbour lattice(it is an atom).
        _type_pair_ia e_des = BondsCounter::count(box.lattice_list, target_lattice.getId(), target_lattice.type);
        e_before = e_src + e_des;
    }

    // exchange atoms of vacancy and neighbour lattice atom/vacancy.
    source_lattice.type = target_lattice.type;
    target_lattice.type = LatticeTypes{trans_atom};

    // calculate system energy after transition.
    double e_after = 0;
    {
        _type_pair_ia e_src = BondsCounter::count(box.lattice_list, source_lattice.getId(), source_lattice.type);
        _type_pair_ia e_des = BondsCounter::count(box.lattice_list, target_lattice.getId(), target_lattice.type);
        e_after = e_src + e_des;
    }

    // exchange atoms back.
    target_lattice.type = source_lattice.type;
    source_lattice.type = LatticeTypes{trans_atom};

    double active_energy = e0 + (e_after - e_before) / 2;
    return arrhenius(box.v, box.T, active_energy);
#endif
}
