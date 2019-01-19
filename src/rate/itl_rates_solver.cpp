//
// Created by genshen on 2019-01-19.
//

#include "itl_rates_solver.h"

ItlRatesSolver::ItlRatesSolver(Box &box) : RatesSolver(box) {

}

_type_rate ItlRatesSolver::rate(const _type_lattice_coord x, const _type_lattice_coord y, const _type_lattice_coord z,
                                const Lattice &source_lattice, const _type_dir_id _1nn_id) {
    // box.lattice_list->getLat(x, y, z);

    return 0;
}
