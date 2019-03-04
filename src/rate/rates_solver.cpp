//
// Created by genshen on 2019-01-10.
//

#include <cmath>
#include "rates_solver.h"

RatesSolver::RatesSolver(LatticesList &lat_list):lattice_list(lat_list) {}

double RatesSolver::activeEnergy() {
    // todo
    return 0;
}

double RatesSolver::arrhenius(const double v, const double T, const double Ea) {
    return v * exp(-Ea / (BoltzmannConstant * T));
}
