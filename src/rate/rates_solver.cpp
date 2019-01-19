//
// Created by genshen on 2019-01-10.
//

#include <cmath>
#include "rates_solver.h"

RatesSolver::RatesSolver(Box &box) : box(box) {}

double RatesSolver::activeEnergy() {
    // todo
    return 0;
}

double RatesSolver::arrhenius(const double v, const double T, const double Ea) {
    return v * exp(-Ea / (BoltzmannConstant * T));
}
