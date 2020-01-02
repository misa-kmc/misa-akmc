//
// Created by genshen on 2019-01-10.
//

#include <cmath>
#include "../env.h"
#include "rates_solver.h"

RatesSolver::RatesSolver(LatticesList &lat_list, const double v, const double T)
        : lattice_list(lat_list), attempt_freq(v), temperature(T) {}

const _type_rate RatesSolver::rate(Lattice &source_lattice, Lattice &target_lattice,
                                   const LatticeTypes::lat_type ghost_atom,
                                   const _type_dir_id _1nn_offset) {
    const _type_lattice_id source_lat_id = source_lattice.getId();
    const _type_lattice_id target_lat_id = lattice_list.meta.getIdBy1nnOffset(source_lat_id, _1nn_offset);
    // calculate active energy first.
    // fixme bug: in e0(), trans atom is vacancy not target atom in transition atom.
    const double active_energy = e0(ghost_atom) + deltaE(source_lat_id, target_lat_id, source_lattice,
                                                         target_lattice, ghost_atom) / 2;
    return arrhenius(attempt_freq, temperature, active_energy);
}

double RatesSolver::arrhenius(const double v, const double T, const double Ea) {
    return v * exp(-Ea / (BoltzmannConstant * T));
}
