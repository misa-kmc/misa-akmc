//
// Created by genshen on 2019-01-19.
//

#include <rate/bonds/bonds_counter.h>
#include "itl_rates_solver.h"

ItlRatesSolver::ItlRatesSolver(Box &box, const double v, const double T) : RatesSolver(box) {
    // todo save T,v
}

_type_rate ItlRatesSolver::rate(const Lattice &source_lattice, const Lattice &target_lattice,
                                const LatticeTypes::lat_type trans_atom,
                                const _type_dir_id _1nn_offset) {
#ifdef EAM_POT_ENABLED
    return 0; // todo eam
#else
    // bonds energy of src lattice contributed by its 1nn/2nn neighbour lattice.
    _type_pair_ia e_src = BondsCounter::count(box.lattice_list, source_lattice.getId(), LatticeTypes{trans_atom});
    // bonds energy of des lattice contributed by its 1nn/2nn neighbour lattice.
    _type_pair_ia e_des = BondsCounter::count(box.lattice_list, _1nn_offset, target_lattice.type);
//    return arrhenius(v,t,);
    return 0;
#endif
}
