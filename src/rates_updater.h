//
// Created by genshen on 2019-01-10.
//

#ifndef MISA_KMC_RATES_UPDATER_H
#define MISA_KMC_RATES_UPDATER_H

#include "box.h"

const double BoltzmannConstant = 1.3806485279; // unit: 10^{-23} J/K

class RatesUpdater {
public:
    _type_rate rate_sum;

    explicit RatesUpdater(Box &box);

private:
    const Box &box;

    /**
     * \brief calculate the rate based on delta e.
     *
     * Arrhenius equation:
     * k=Ae^{ frac{ -E_a }{ K_bT } }
     * see https://en.wikipedia.org/wiki/Arrhenius_equation for more details.
     *
     * \param v attempt rate
     * \param T temperature
     * \param Ea activation energy for the reaction (in the same units as kB*T)
     * \return the transition rate
     *
     */
    double arrhenius(const double v, const double T, const double delta_e);

    /**
     * return delta E
     */
    double activeEnergy();

};

#endif //MISA_KMC_RATES_UPDATER_H
