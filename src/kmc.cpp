//
// Created by genshen on 2018-12-12.
//

#include "kmc.h"
#include "rates_updater.h"

void kmc::updateRates(double v, double T) {
    RatesUpdater updater(*box);
    box->lattice_list->forAllLattices([&, v, T](const _type_lattice_coord x,
                                                const _type_lattice_coord y,
                                                const _type_lattice_coord z,
                                                Lattice &lattice) {
        if (lattice.type.isAtom()) {
            // todo
        } else if (lattice.type.isDumbbell()) { // dumbbell
            Itl &itl_ref = box->itl_list->mp.at(lattice.getId());

            Lattice *lat_list[8];
            box->lattice_list->get1nn(x, y, z, lat_list);
            _type_neighbour_status nei_status = box.lattice_list->get1nnStatus(x, y, z);
            // update transition rate to each direction
            itl_ref.updateRates(lat_list, nei_status);
        } else { // vacancy
            Lattice *lat_list[8];
            int size_1nn = box->lattice_list->get1nn(x, y, z, lat_list);
            Vacancy &vacancy = box->va_list->mp.at(lattice.getId());
            for (int i = 0; i < size_1nn; i++) {
                double Ea = activeEnergy();
                vacancy.rates[i] = arrhenius(v, T, Ea);
            }
        }
        return true;
    });
}
