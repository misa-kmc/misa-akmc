//
// Created by genshen on 2019-01-19.
//

#include <cassert>

#include "env.h"
#include "rate/bonds/bonds_counter.h"
#include "itl_rates_solver.h"
#include "rate/bonds/energy_dumbbell.h"

ItlRatesSolver::ItlRatesSolver(Box &box, const double v, const double T) : RatesSolver(box) {
    // todo save T,v
}

_type_rate ItlRatesSolver::rate(Lattice &source_lattice, Lattice &target_lattice,
                                const LatticeTypes::lat_type trans_atom,
                                const _type_dir_id _1nn_offset) {
#ifdef DEBUG_MODE
    {
        const bool debug_bool = target_lattice.type.isAtom();
        assert(debug_bool);
    };
#endif

#ifdef EAM_POT_ENABLED
    return 0; // todo eam
#else
    double e0 = 0;
    switch (trans_atom) {
        case LatticeTypes::Fe:
            e0 = 0.32;
            break;
        case LatticeTypes::Cu:
            e0 = 0.36;
            break;
        case LatticeTypes::Ni:
            e0 = 0.45;
            break;
        case LatticeTypes::Mn:
            e0 = 0.31;
            break;
        default:
            e0 = 0;
            break;
    }

    //calculate system energy before transition.
    double e_before = 0;
    {
        // bonds energy of src lattice contributed by its 1nn/2nn neighbour lattice.
        bonds::_type_pair_ia e_src = bonds::BondsCounter::count(box.lattice_list,
                                                                source_lattice.getId(),
                                                                LatticeTypes{trans_atom});
        // bonds energy of des lattice contributed by its 1nn/2nn neighbour lattice(it is an atom).
        bonds::_type_pair_ia e_des = bonds::BondsCounter::count(box.lattice_list,
                                                                target_lattice.getId(),
                                                                target_lattice.type);
        // the count of dumbbells does not change, so we does not count the term: sum_itl *Ef110.
        double e_dumbbell = bond::Edumb(*box.lattice_list, *box.itl_list);
        e_before = e_src + e_des + e_dumbbell;
    }

    // exchange atoms of jump atom and neighbour lattice atom.
    const LatticeTypes cached_source_type = source_lattice.type;
    const LatticeTypes cached_target_type = target_lattice.type;
    source_lattice.type._type = source_lattice.type.diff(LatticeTypes{trans_atom});
    target_lattice.type._type = target_lattice.type.combineToInter(trans_atom);

    double e_after = 0;
    {
        bonds::_type_pair_ia e_src = bonds::BondsCounter::count(box.lattice_list,
                                                                source_lattice.getId(),
                                                                source_lattice.type);
        bonds::_type_pair_ia e_des = bonds::BondsCounter::count(box.lattice_list,
                                                                target_lattice.getId(),
                                                                LatticeTypes{trans_atom});
        // the count of dumbbells does not change, so we does not count the term: sum_itl *Ef110.
        double e_dumbbell = bond::Edumb(*box.lattice_list, *box.itl_list);
        e_after = e_src + e_des + e_dumbbell;
    }

    // exchange atoms back.
    source_lattice.type = cached_source_type;
    target_lattice.type = cached_target_type;

    double active_energy = e0 + (e_after - e_before) / 2;
    return arrhenius(env::global_env.attempt_freq, env::global_env.temperature, active_energy);
#endif
}
