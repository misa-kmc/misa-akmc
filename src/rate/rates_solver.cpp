//
// Created by genshen on 2019-01-10.
//

#include <cmath>
#include "env.h"
#include "rates_solver.h"

RatesSolver::RatesSolver(LatticesList &lat_list) : lattice_list(lat_list) {}

const _type_rate RatesSolver::rate(Lattice &source_lattice, Lattice &target_lattice,
                             const LatticeTypes::lat_type trans_atom,
                             const _type_dir_id _1nn_offset) {
    // calculate active energy first.
    // fixme bug: in e0(), trans atom is vacancy not target atom in transition atom.
    const double active_energy = e0(trans_atom) + deltaE(source_lattice, target_lattice, trans_atom) / 2;
    return arrhenius(env::global_env.attempt_freq, env::global_env.temperature, active_energy);;
}

double RatesSolver::arrhenius(const double v, const double T, const double Ea) {
    return v * exp(-Ea / (BoltzmannConstant * T));
}
