//
// Created by genshen on 2019-01-10.
//

#ifndef MISA_KMC_RATES_UPDATER_H
#define MISA_KMC_RATES_UPDATER_H

#include "box.h"

const double BoltzmannConstant = 1.3806485279; // unit: 10^{-23} J/K

class RatesUpdater {
    _type_rate rate_sum;

    explicit RatesUpdater(Box &box);

    /**
     * \brief calculate the transition rates of each lattice.
     *
     * The calculating methods depends on the lattice type(single atom, vacancy and dumbbell).
     * Different types of lattice have different methods or formulas to calculate the rate.
     * see the implementation for more details.
     *
     * After this step, the rate of every transition direction of each lattice will be set.
     */
    void updateRates(double v, double T);

private:
    const Box &box;

    /**
     * return delta E
     */
    double deltaE();

};

#endif //MISA_KMC_RATES_UPDATER_H
