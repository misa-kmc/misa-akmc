//
// Created by genshen on 2018-12-12.
//

#include "kmc.h"
#include "rate/itl_rates_solver.h"

void kmc::updateRates(double v, double T) {
    ItlRatesSolver itl_rate(*box, v, T);
    box->lattice_list->forAllLattices([&](const _type_lattice_coord x,
                                          const _type_lattice_coord y,
                                          const _type_lattice_coord z,
                                          Lattice &lattice) {
        if (lattice.type.isAtom()) {
            // todo
        } else if (lattice.type.isDumbbell()) { // dumbbell
            Itl &itl_ref = box->itl_list->mp.at(lattice.getId());

            Lattice *lat_list[LatticesList::MAX_1NN]; // todo new array many times.
            box->lattice_list->get1nn(x, y, z, lat_list);
            _type_neighbour_status nei_status = box->lattice_list->get1nnStatus(x, y, z);
            itl_ref.beforeRatesUpdate(lat_list, nei_status);
            // update transition rate to each direction
            itl_ref.updateRates(lattice, lat_list, nei_status,
                                [&lattice, &itl_rate](const Lattice *lat_nei,
                                                      const LatticeTypes::lat_type trans_atom,
                                                      const _type_dir_id _1nn_offset) -> _type_rate {
                                    // in lambda, it returns the rate of transition
                                    // from current lattice to lat_nei neighbour lattice.
                                    return itl_rate.rate(lattice, *lat_nei, trans_atom, _1nn_offset);
                                });
        } else { // vacancy
            Lattice *lat_list[LatticesList::MAX_1NN]; // todo new array many times.
            int size_1nn = box->lattice_list->get1nn(x, y, z, lat_list);
            Vacancy &vacancy = box->va_list->mp.at(lattice.getId());
            for (int i = 0; i < size_1nn; i++) {
                // todo
//                double Ea = activeEnergy();
//                vacancy.rates[i] = arrhenius(v, T, Ea);
            }
        }
        return true;
    });
}
