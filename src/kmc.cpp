//
// Created by genshen on 2018-12-12.
//

#include "kmc.h"
#include "rate/itl_rates_solver.h"
#include "rate/vacancy_rates_solver.h"

_type_rate kmc::updateRates(double v, double T) {
    _type_rate sum_rates = 0;
    ItlRatesSolver itl_rate(*box, v, T);
    VacRatesSolver vac_rate(*box);

    box->lattice_list->forAllLattices([&](const _type_lattice_coord x,
                                          const _type_lattice_coord y,
                                          const _type_lattice_coord z,
                                          Lattice &lattice) {
        if (lattice.type.isDumbbell()) { // dumbbell
            Itl &itl_ref = box->itl_list->mp.at(lattice.getId());

            Lattice *lat_list[LatticesList::MAX_1NN]; // todo new array many times.
            box->lattice_list->get1nn(x, y, z, lat_list);
            _type_neighbour_status nei_status = box->lattice_list->get1nnStatus(x, y, z);
            itl_ref.beforeRatesUpdate(lat_list, nei_status);
            // update transition rate to each direction
            itl_ref.updateRates(lattice, lat_list, nei_status,
                                [&lattice, &itl_rate, &sum_rates]
                                        (Lattice *lat_nei,
                                         const LatticeTypes::lat_type trans_atom,
                                         const _type_dir_id _1nn_offset) -> _type_rate {
                                    // in lambda, it returns the rate of transition
                                    // from current lattice to lat_nei neighbour lattice.
                                    _type_rate rate = itl_rate.rate(lattice, *lat_nei, trans_atom, _1nn_offset);
                                    sum_rates += rate; // add this rate to sum
                                    return rate;
                                });
        } else if (lattice.type.isVacancy()) { // vacancy
            Vacancy &vacancy = box->va_list->mp.at(lattice.getId());

            Lattice *lat_list[LatticesList::MAX_1NN]; // todo new array many times.
            box->lattice_list->get1nn(x, y, z, lat_list);
            _type_neighbour_status nei_status = box->lattice_list->get1nnStatus(x, y, z);
            vacancy.beforeRatesUpdate(lat_list, nei_status);
            // update transition rate to each direction
            vacancy.updateRates(lattice, lat_list, nei_status,
                                [&lattice, &vac_rate, &sum_rates]
                                        (Lattice *lat_nei,
                                         const LatticeTypes::lat_type trans_atom,
                                         const _type_dir_id _1nn_offset) -> _type_rate {
                                    _type_rate rate = vac_rate.rate(lattice, *lat_nei, trans_atom, _1nn_offset);
                                    sum_rates += rate; // add this rate to sum
                                    return rate;
                                });
        }
        // there is no transition rate for single atom.
        // todo rate for defect generation.
        return true;
    });
    return sum_rates;
}

event::SelectedEvent kmc::select(const double random, const _type_rate sum_rates) {
    const _type_rate excepted_rate = sum_rates * random;
    _type_rate rate_accumulator = 0.0;
    event::SelectedEvent selected_event{event::DefectGen, 0, 0}; // default event is defect generation.
    box->lattice_list->forAllLattices([&](const _type_lattice_coord x,
                                          const _type_lattice_coord y,
                                          const _type_lattice_coord z,
                                          Lattice &lattice) {
        if (lattice.type.isDumbbell()) { // dumbbell
            const Itl &itl_ref = box->itl_list->mp.at(lattice.getId());
            for (int i = 0; i < Itl::RATES_SIZE; i++) {
                rate_accumulator += itl_ref.rates[i];
                if (rate_accumulator > excepted_rate) {
                    selected_event.id = lattice.getId();
                    selected_event.event_type = event::DumbbellTrans;
                    selected_event.rate_index = i;
                    return false;
                }
            }
        } else if (lattice.type.isVacancy()) {
            const Vacancy &vacancy = box->va_list->mp.at(lattice.getId());
            for (int i = 0; i < Vacancy::RATES_SIZE; i++) {
                rate_accumulator += vacancy.rates[i];
                if (rate_accumulator > excepted_rate) {
                    selected_event.id = lattice.getId();
                    selected_event.event_type = event::VacancyTrans;
                    selected_event.rate_index = i;
                    return false;
                }
            }
        }
        return true;
    });
#ifdef DEBUG_MODE
//    todo assert total rates == rate_accumulator + defect_gen_rate
#endif
    return selected_event;
}
