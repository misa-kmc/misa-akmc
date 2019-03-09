//
// Created by genshen on 2018-12-12.
//

#include <cassert>

#include "kmc.h"
#include "rate/itl_rates_solver.h"
#include "rate/vacancy_rates_solver.h"
#include "recombine.h"
#include "utils/random/random.h"
#include "env.h"

kmc::kmc(Box *box) : box(box) {}

_type_rate kmc::updateRates(double v, double T) {
    _type_rate sum_rates = 0;
    ItlRatesSolver itl_rate(*(box->lattice_list), *(box->va_list), *(box->itl_list), v, T);
    VacRatesSolver vac_rate(*(box->lattice_list), v, T);

    box->lattice_list->forAllLattices([&](const _type_lattice_coord x,
                                          const _type_lattice_coord y,
                                          const _type_lattice_coord z,
                                          Lattice &lattice) {
        if (lattice.type.isDumbbell()) { // dumbbell
            Itl &itl_ref = box->itl_list->mp.at(lattice.getId());

            Lattice *lat_list[LatticesList::MAX_1NN] = {nullptr}; // todo new array many times.
            box->lattice_list->get1nn(x, y, z, lat_list);
            _type_neighbour_status nei_status = box->lattice_list->get1nnStatus(x, y, z);
            itl_ref.beforeRatesUpdate(lat_list, nei_status);
            // update transition rate to each direction
            itl_ref.updateRates(lattice, lat_list, nei_status,
                                [&lattice, &itl_rate, &sum_rates]
                                        (Lattice *lat_nei,
                                         const LatticeTypes::lat_type ghost_atom,
                                         const _type_dir_id _1nn_offset) -> _type_rate {
                                    // in lambda, it returns the rate of transition
                                    // from current lattice to lat_nei neighbour lattice.
                                    _type_rate rate = itl_rate.rate(lattice, *lat_nei, ghost_atom, _1nn_offset);
                                    sum_rates += rate; // add this rate to sum
                                    return rate;
                                });
        } else if (lattice.type.isVacancy()) { // vacancy
            Vacancy &vacancy = box->va_list->mp.at(lattice.getId());

            Lattice *lat_list[LatticesList::MAX_1NN] = {nullptr}; // todo new array many times.
            box->lattice_list->get1nn(x, y, z, lat_list);
            _type_neighbour_status nei_status = box->lattice_list->get1nnStatus(x, y, z);
            vacancy.beforeRatesUpdate(lat_list, nei_status);
            // update transition rate to each direction
            vacancy.updateRates(lattice, lat_list, nei_status,
                                [&lattice, &vac_rate, &sum_rates]
                                        (Lattice *lat_nei,
                                         const LatticeTypes::lat_type ghost_atom,
                                         const _type_dir_id _1nn_offset) -> _type_rate {
                                    _type_rate rate = vac_rate.rate(lattice, *lat_nei, ghost_atom, _1nn_offset);
                                    sum_rates += rate; // add this rate to sum
                                    return rate;
                                });
        }
        // there is no transition rate for single atom.
        return true;
    });
    sum_rates += defectGenRate();
    return sum_rates;
}

_type_rate kmc::defectGenRate() {
    return env::global_env.defect_gen_rate;
}

event::SelectedEvent kmc::select(const double excepted_rate, const _type_rate sum_rates) {
    _type_rate rate_accumulator = 0.0;
    event::SelectedEvent selected_event{event::DefectGen, 0, 0}; // default event is defect generation.
    box->lattice_list->forAllLattices([&](const _type_lattice_coord x,
                                          const _type_lattice_coord y,
                                          const _type_lattice_coord z,
                                          Lattice &lattice) {
        if (lattice.type.isDumbbell()) { // dumbbell
            const Itl &itl_ref = box->itl_list->mp.at(lattice.getId());
            for (int rate_index = 0; rate_index < Itl::RATES_SIZE; rate_index++) {
                rate_accumulator += itl_ref.rates[rate_index];
                if (rate_accumulator > excepted_rate) {
                    Lattice *_1nn_list[LatticesList::MAX_1NN] = {nullptr};
                    box->lattice_list->get1nn(lattice.getId(), _1nn_list);
                    // convert rate index to 1nn list array index.
                    const orientation ori = box->itl_list->mp[lattice.getId()].orient;
                    const _type_dir_id _1nn_tag = Itl::get1nnIdByRatesIndex(rate_index, ori.availTransDirs());
#ifdef DEBUG_MODE
                    assert(_1nn_list[_1nn_tag] != nullptr);
#endif
                    selected_event.event_type = event::DumbbellTrans;
                    selected_event.from_id = lattice.getId();
                    selected_event.to_id = _1nn_list[_1nn_tag]->getId();
                    selected_event.target_tag = _1nn_tag;
                    selected_event.rotate_direction = rate_index % 2 != 0;
                    return false;
                }
            }
        } else if (lattice.type.isVacancy()) {
            const Vacancy &vacancy = box->va_list->mp.at(lattice.getId());
            for (int rate_index = 0; rate_index < Vacancy::RATES_SIZE; rate_index++) {
                rate_accumulator += vacancy.rates[rate_index];
                if (rate_accumulator > excepted_rate) {
                    Lattice *_1nn_list[LatticesList::MAX_1NN] = {nullptr};
                    box->lattice_list->get1nn(lattice.getId(), _1nn_list);
#ifdef DEBUG_MODE
                    assert(_1nn_list[rate_index] != nullptr);
#endif
                    selected_event.event_type = event::VacancyTrans;
                    selected_event.from_id = lattice.getId();
                    selected_event.to_id = _1nn_list[rate_index]->getId();
                    selected_event.target_tag = static_cast<_type_dir_id>(rate_index);
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
            Lattice &lat_from = box->lattice_list->getLat(selected.from_id);
            Lattice &lat_to = box->lattice_list->getLat(selected.to_id);
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
            if (p_event_listener) {
                p_event_listener->onVacancyTrans(0, lat_from.type);
            }
            // recombination
            rec::RecList rec_list;
            rec_list.create(box->lattice_list, box->itl_list, lat_to.getId());
            if (!rec_list.rec_list.empty()) {
                rec::Rec picked_rec = rec_list.pickMinimum();
                if (p_event_listener) {
                    p_event_listener->onRecombine(0, picked_rec);
                }
                picked_rec.recombine(box->lattice_list, box->va_list, box->itl_list);
            }
        }
            break;
        case event::DumbbellTrans: {
            Lattice &lat_from = box->lattice_list->getLat(selected.from_id);
            Lattice &lat_to = box->lattice_list->getLat(selected.to_id);
            Itl itl_copy = box->itl_list->mp.at(selected.from_id);
#ifdef DEBUG_MODE
            assert(lat_from.type.isDumbbell());
            assert(lat_to.type.isAtom());
#endif
            // find jump atom and exchange atoms.
            const LatticeTypes old_from_type = lat_from.type;
            const LatticeTypes old_to_type = lat_to.type;
            LatticeTypes jump_atom = itl_copy.orient.tranAtom(lat_from.type,
                                                              selected.target_tag); // for example: jump_atom = X
            lat_from.type._type = lat_from.type.diff(LatticeTypes{jump_atom});  // XY -> Y
            lat_to.type._type = LatticeTypes::combineToInter(lat_to.type._type, jump_atom._type); // N -> NX or N -> XN
            // update orientation
            Itl itl;
            itl.orient = itl_copy.orient.trans(selected.target_tag,
                                               lat_to.type.isHighEnd(jump_atom._type),
                                               selected.rotate_direction);
            // todo update avail tran dirs, not in beforeRatesUpdate.
            box->itl_list->replace(lat_from.getId(), lat_to.getId(), itl);
            if (p_event_listener) {
                p_event_listener->onDumbbellTrans(0, old_from_type, old_to_type, lat_from.type, lat_to.type);
            }
            // recombination
            rec::RecList rec_list;
            rec_list.create(box->lattice_list, box->itl_list, lat_to.getId());
            if (!rec_list.rec_list.empty()) {
                rec::Rec picked_rec = rec_list.pickMinimum();
                if (p_event_listener) {
                    p_event_listener->onRecombine(0, picked_rec);
                }
                picked_rec.recombine(box->lattice_list, box->va_list, box->itl_list);
            }
        }
            break;
        case event::DefectGen: {
            const uint32_t it_start = r::rand32(0, static_cast<const uint32_t>(box->lattice_list->getLatCount()));
            static const uint32_t dfp = 4;
            // search over all lattice to find the first lattice that satisfies the condition.
            Lattice *lat_1 = nullptr, *lat_2 = nullptr;
            for (_type_lattice_id local_id = it_start;
                 local_id < it_start + box->lattice_list->getLatCount(); local_id++) {
                // the lattice type must be Fe, and its neighbour (with distance dfp) can be Fe,Cu,Mn,Ni
                // note: the distance is Manhattan distance, not Euclidean distance。
                Lattice &lat = box->lattice_list->getLat(local_id); // todo use global id.
                lat_1 = &lat;
                if (lat.type._type == LatticeTypes::Fe) {
                    // traversing it neighbour with Manhattan distance dfp.
                    for (int32_t x = 0; x <= dfp; x++) {
                        for (int32_t y = 0; y <= dfp - x; y++) {
                            const int32_t z = dfp - x - y;
                            lat_2 = box->lattice_list->walk(local_id, x, y, z);
                            if (lat_2 != nullptr && lat_2->type.isAtom()) {
                                goto BK_SEARCH;
                            }
                            lat_2 = box->lattice_list->walk(local_id, x, y, -z);
                            if (lat_2 != nullptr && lat_2->type.isAtom()) {
                                goto BK_SEARCH;
                            }
                            lat_2 = box->lattice_list->walk(local_id, x, -y, z);
                            if (lat_2 != nullptr && lat_2->type.isAtom()) {
                                goto BK_SEARCH;
                            }
                            lat_2 = box->lattice_list->walk(local_id, x, -y, -z);
                            if (lat_2 != nullptr && lat_2->type.isAtom()) {
                                goto BK_SEARCH;
                            }
                            lat_2 = box->lattice_list->walk(local_id, -x, y, z);
                            if (lat_2 != nullptr && lat_2->type.isAtom()) {
                                goto BK_SEARCH;
                            }
                            lat_2 = box->lattice_list->walk(local_id, -x, y, -z);
                            if (lat_2 != nullptr && lat_2->type.isAtom()) {
                                goto BK_SEARCH;
                            }
                            lat_2 = box->lattice_list->walk(local_id, -x, -y, z);
                            if (lat_2 != nullptr && lat_2->type.isAtom()) {
                                goto BK_SEARCH;
                            }
                            lat_2 = box->lattice_list->walk(local_id, -x, -y, -z);
                            if (lat_2 != nullptr && lat_2->type.isAtom()) {
                                goto BK_SEARCH;
                            }
                        }
                    }
                }
            }
            BK_SEARCH:
            if (lat_1 == nullptr || lat_2 == nullptr) {
                // error: not searched. todo
            }
            // randomly select one lattice to be vacancy, another to be dumbbell.
            const uint32_t rand = r::rand32(0, 1);
            if (rand == 1) {
                Lattice *temp = lat_1;
                lat_1 = lat_2;
                lat_2 = temp;
            }
            const LatticeTypes::lat_type combined_type =
                    LatticeTypes::combineToInter(lat_1->type._type, lat_2->type._type);
            if (p_event_listener) {
                p_event_listener->onDefectGenerate(0, lat_1->type, lat_2->type, combined_type);
            }
            lat_1->setType(combined_type);
            lat_2->setType(LatticeTypes::V);
            // todo: consider its neighbour, and recombine.

            // update orientation
            Itl itl;
            itl.orient = orientation{orientation::random()};
            // todo update avail tran dirs, not in beforeRatesUpdate.
            box->itl_list->mp.insert(std::make_pair(lat_1->getId(), itl));
            box->va_list->mp.insert(std::make_pair(lat_2->getId(), Vacancy{}));
        }
            break;
    }
}
