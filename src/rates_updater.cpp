//
// Created by genshen on 2019-01-10.
//

#include <cmath>
#include "rates_updater.h"
#include "box.h"
#include "defect/vacancy_list.h"
#include "defect/itl_list.h"


RatesUpdater::RatesUpdater(Box &box) : box(box) {}

double RatesUpdater::activeEnergy() {
    // todo
    return 0;
}

double RatesUpdater::arrhenius(const double v, const double T, const double Ea) {
    return v * exp(-Ea / (BoltzmannConstant * T));
}
