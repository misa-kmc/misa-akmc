//
// Created by genshen on 2019-01-10.
//

#include <cmath>
#include "box.h"
#include "rates_updater.h"


RatesUpdater::RatesUpdater(Box &box) : box(box) {}

void RatesUpdater::updateRates(double v, double T) {
    box.lattice_list->forAllLattices([&, v, T](const _type_lattice_coord x,
                                               const _type_lattice_coord y,
                                               const _type_lattice_coord z,
                                               Lattice &lattice) {
        if (lattice.type.isAtom()) {
            // todo
        } else if (lattice.type.isDumbbell()) {
            // todo
        } else { // vacancy
            Lattice *lat_list[8];
            int size_1nn = box.lattice_list->get1nn(x, y, z, lat_list);
            Vacancy &vacancy = box.va_list->mp.at(lattice.getId());
            for (int i = 0; i < size_1nn; i++) {
                double delta_e = deltaE();
                vacancy.rates.r[i] = v * exp(-delta_e / (BoltzmannConstant * T));
            }
        }
        return true;
    });
}

double RatesUpdater::deltaE() {
    // todo
    return 0;
}
