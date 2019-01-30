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

void kmc::execute(const event::SelectedEvent selected) {
    switch (selected.event_type) {
        case event::VacancyTrans: {
            Lattice &lat_from = box->lattice_list->getLat(selected.id);
            Lattice *_1nn_list[LatticesList::MAX_1NN] = {nullptr};
            box->lattice_list->get1nn(selected.id, _1nn_list);
            Lattice &lat_to = *(_1nn_list[selected.rate_index]);
#ifdef DEBUG_MODE
            assert(lat_from.type.isVacancy());
            assert(lat_to.type.isAtom());
#endif
            // exchange lattice type
            LatticeTypes tp_temp = lat_from.type;
            lat_from.type = lat_to.type;
            lat_to.type = tp_temp;
            // update vacancies list: remove lat_from, add lat_to.
            box->va_list->replace(lat_from.getId(), lat_to.getId());
            // recombination
        }
            break;
        case event::DumbbellTrans: {
            Lattice &lat_from = box->lattice_list->getLat(selected.id);
            Lattice *_1nn_list[LatticesList::MAX_1NN] = {nullptr};
            box->lattice_list->get1nn(selected.id, _1nn_list);
            // convert rate index to 1nn list array index.
            const orientation ori = box->itl_list->mp[selected.id].orient;
            const _type_dir_id _1nn_tag = Itl::get1nnIdByRatesIndex(
                    selected.rate_index,
                    ori.availTransDirs());
            Lattice &lat_to = *(_1nn_list[_1nn_tag]);
#ifdef DEBUG_MODE
            assert(lat_from.type.isDumbbell());
            assert(lat_to.type.isAtom());
#endif
            // find jump atom and exchange atoms.
            LatticeTypes jump_atom = ori.tranAtom(lat_from.type, _1nn_tag); // for example: jump_atom = X
            lat_from.type._type = lat_from.type.diff(LatticeTypes{jump_atom});  // XY -> Y
            lat_to.type._type = LatticeTypes::combineToInter(lat_to.type._type,
                                                             jump_atom._type); // N -> NX or N -> XN
            // update orientation
            Itl itl;
            itl.orient = ori.trans(_1nn_tag, lat_to.type.isHighEnd(jump_atom._type), _1nn_tag % 2 != 0);
            // todo update avail tran dirs.
            box->itl_list->replace(lat_from.getId(), lat_from.getId(), itl);
            // recombination
        }
            break;
        case event::DefectGen: {
// todo
        }
            break;
    }
}
