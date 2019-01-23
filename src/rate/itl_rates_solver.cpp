//
// Created by genshen on 2019-01-19.
//

#include "rate/bonds/bonds_counter.h"
#include "itl_rates_solver.h"

ItlRatesSolver::ItlRatesSolver(Box &box, const double v, const double T) : RatesSolver(box) {
    // todo save T,v
}

_type_rate ItlRatesSolver::rate(Lattice &source_lattice, Lattice &target_lattice,
                                const LatticeTypes::lat_type trans_atom,
                                const _type_dir_id _1nn_offset) {
#ifdef EAM_POT_ENABLED
    return 0; // todo eam
#else
    double e0 = 0;
    switch (trans_atom) {
        case LatticeTypes::Fe:
            e0 = 0.32;
            break;
        case LatticeTypes::Cu:
            e0 = 0.36;
            break;
        case LatticeTypes::Ni:
            e0 = 0.45;
            break;
        case LatticeTypes::Mn:
            e0 = 0.31;
            break;
        default:
            e0 = 0;
            break;
    }

    //calculate system energy before transition.
    double e_before = 0;
    {
    // bonds energy of src lattice contributed by its 1nn/2nn neighbour lattice.
    _type_pair_ia e_src = BondsCounter::count(box.lattice_list, source_lattice.getId(), LatticeTypes{trans_atom});
        // bonds energy of des lattice contributed by its 1nn/2nn neighbour lattice(it is an atom).
        _type_pair_ia e_des = BondsCounter::count(box.lattice_list, target_lattice.getId(), target_lattice.type);
        double e_dumbbell = Edumb(); // the count of dumbbells does not change, so we does not count this term.
        e_before = e_src + e_des + e_dumbbell;
    }

    // exchange atoms of jump atom and neighbour lattice atom.
    const LatticeTypes cached_source_type = source_lattice.type;
    const LatticeTypes cached_target_type = target_lattice.type;
    source_lattice.type._type = source_lattice.type.diff(LatticeTypes{trans_atom});
    target_lattice.type._type = target_lattice.type.combineToInter(trans_atom);

    double e_after = 0;
    {
        _type_pair_ia e_src = BondsCounter::count(box.lattice_list, source_lattice.getId(), source_lattice.type);
        _type_pair_ia e_des = BondsCounter::count(box.lattice_list, target_lattice.getId(), LatticeTypes{trans_atom});
        double e_dumbbell = Edumb(); // the count of dumbbells does not change, so we does not count this term.
        e_after = e_src + e_des + e_dumbbell;
    }

    // exchange atoms back.
    source_lattice.type = cached_source_type;
    target_lattice.type = cached_target_type;

    double active_energy = e0 + (e_after - e_before) / 2;
    return arrhenius(box.v, box.T, active_energy);
#endif
}

double ItlRatesSolver::Edumb() {
    // used in each inter loop.
    Lattice *_1nn_list[LatticesList::MAX_1NN];
    Lattice *_2nn_list[LatticesList::MAX_2NN];

    double edumb = 0;
    auto _1nn_2nn_contribute = [](Lattice **_1nns, Lattice **_2nns,
                                  _type_neighbour_status _1nn_status,
                                  _type_neighbour_status _2nn_status) -> double {
        double contr = 0;
        for (int b = 0; b < LatticesList::MAX_NEI_BITS; b++) {
            if (((_1nn_status >> b) & 0x01) && _1nns[b]->type.isDumbbell()) {
                // the 1nn neighbour exists and it is also dumbbell
                contr += 0.70;
            }
            if (((_2nn_status >> b) & 0x01) && _2nns[b]->type.isDumbbell()) {
                // the 2nn neighbour exists and it is also dumbbell
                contr += 0.48;
            }
        }
        return contr;
    };

    for (auto const &itl_pair : box.itl_list->mp) {
        // get lattice id, inter instance, and lattice type
        const _type_lattice_id &id = itl_pair.first;
        const Itl &inter_instance = itl_pair.second;
        const LatticeTypes lat_type = box.lattice_list->getLat(id).type;

        // get 1nn/2nn lists as well as status of this lattice
        const _type_neighbour_status _1nn_status = box.lattice_list->get1nnStatus(id);
        const _type_neighbour_status _2nn_status = box.lattice_list->get2nnStatus(id);
        box.lattice_list->get1nn(id, _1nn_list);
        box.lattice_list->get2nn(id, _2nn_list);

        double emix = 0;
        double etens = 0;
        double ecomp = 0;
        double eb = 0;

        switch (lat_type._type) {
            case LatticeTypes::FeCu:
                emix = -0.46;
                ecomp = FeX_comp(id, lat_type, inter_instance, _1nn_list, _2nn_list, _1nn_status, _2nn_status);
                break;
            case LatticeTypes::CuCu:
                emix = -0.36;
                eb += _1nn_2nn_contribute(_1nn_list, _2nn_list, _1nn_status, _2nn_status);
                break;
            case LatticeTypes::FeNi:
                emix = -0.19;
                ecomp = FeX_comp(id, lat_type, inter_instance, _1nn_list, _2nn_list, _1nn_status, _2nn_status);
                break;
            case LatticeTypes::CuNi:
                emix = -0.5;
                eb += _1nn_2nn_contribute(_1nn_list, _2nn_list, _1nn_status, _2nn_status);
                break;
            case LatticeTypes::NiNi:
                emix = -0.3;
                eb += _1nn_2nn_contribute(_1nn_list, _2nn_list, _1nn_status, _2nn_status);
                break;
            case LatticeTypes::FeMn:
                emix = 0.39;
                ecomp = FeX_comp(id, lat_type, inter_instance, _1nn_list, _2nn_list, _1nn_status, _2nn_status);
                break;
            case LatticeTypes::CuMn:
                emix = -0.11;
                eb += _1nn_2nn_contribute(_1nn_list, _2nn_list, _1nn_status, _2nn_status);
                break;
            case LatticeTypes::NiMn:
                emix = 0.08;
                eb += _1nn_2nn_contribute(_1nn_list, _2nn_list, _1nn_status, _2nn_status);
                break;
            case LatticeTypes::MnMn:
                emix = 0.41;
                eb += _1nn_2nn_contribute(_1nn_list, _2nn_list, _1nn_status, _2nn_status);
                break;
            case LatticeTypes::FeFe:
                etens = 0;
                // for 1nn
                for (int b = 0; b < LatticesList::MAX_NEI_BITS; b++) {
                    if ((_1nn_status >> b) & 0x01) { //  the 1nn neighbour exists
                        if (!((inter_instance.orientation.orient.availTransDirections() >> b) & 0x01)) {
                            // it is not transition direction.
                            // the second condition can also be:
                            // (inter_instance.avail_trans_dir >> b) & 0x01
                            if (_1nn_list[b]->type.isDumbbell()) {
                                eb += 0.70;
                            } else {
                                if (_1nn_list[b]->type._type == LatticeTypes::Cu) {
                                    etens += 0.13;
                                } else if (_1nn_list[b]->type._type == LatticeTypes::Ni) {
                                    etens += 0.02;
                                } else if (_1nn_list[b]->type._type == LatticeTypes::Mn) {
                                    etens += -0.36;
                                }
                            }
                        } else { // it is transition direction.
                            if (_1nn_list[b]->type.isDumbbell()) {
                                eb += 0.70;
                            } else {
                                if (_1nn_list[b]->type._type == LatticeTypes::Cu) {
                                    etens += -0.13;
                                } else if (_1nn_list[b]->type._type == LatticeTypes::Ni) {
                                    etens += 0.15;
                                } else if (_1nn_list[b]->type._type == LatticeTypes::Mn) {
                                    etens += 0.02;
                                }
                            }
                        }
                    }
                }
                // for 2nn
                for (int b = 0; b < LatticesList::MAX_NEI_BITS; b++) {
                    if (((_2nn_status >> b) & 0x01) && _1nn_list[b]->type.isDumbbell()) {
                        eb += 0.48;
                    }
                }
                break;
            default:
                emix = 0;
                etens = 0;
                ecomp = 0;
                eb = 0;
                break;
        }
        // eb is added twice, so we divided 2.
        edumb += emix + etens + ecomp + eb / 2;
    }
    return edumb;
}

double ItlRatesSolver::FeX_comp(const _type_lattice_id id, const LatticeTypes type,
                                const Itl &itl,
                                Lattice *_1nns[LatticesList::MAX_1NN],
                                Lattice *_2nns[LatticesList::MAX_2NN],
                                const _type_neighbour_status _1nn_status,
                                const _type_neighbour_status _2nn_status) {
    double ecomp_FeX = 0;
    double eb_FeX = 0;
    _type_dirs_status comp_status = itl.avail_trans_dir; // todo make sure avail_trans_dir have been set.
    LatticeTypes compsol;
    if (type.getFirst(itl.orientation.reversed) == LatticeTypes::Fe) {
        comp_status &= 0xF0; // set 4 lower bits to 0.
        compsol = LatticeTypes{type.getSecond(itl.orientation.reversed)};
    }
    if (type.getSecond(itl.orientation.reversed) == LatticeTypes::Fe) {
        comp_status &= 0x0F; // set 4 higher bits to 0.
        compsol = LatticeTypes{type.getFirst(itl.orientation.reversed)};

    }

    // for 1nn neighbour lattice
    for (int b = 0; b < Itl::TRANS_DIRS_BITS_SIZE; b++) {
        if ((comp_status >> b) & 0x01) {
            // the neighbour exists, and it is the transition direction atom X can jump to.
            // and X is not atom Fe(which must be true).
            switch (compsol._type) {
                case LatticeTypes::Cu:
                    if (_1nns[b]->type.isDumbbell()) {
                        eb_FeX += 0.70;
                    } else {
                        if (_1nns[b]->type._type == LatticeTypes::Cu) {
                            ecomp_FeX += -0.18;
                        } else if (_1nns[b]->type._type == LatticeTypes::Ni) {
                            ecomp_FeX += -0.35;
                        } else if (_1nns[b]->type._type == LatticeTypes::Mn) {
                            ecomp_FeX += -0.46;
                        }
                    }
                    break;
                case LatticeTypes::Ni:
                    if (_1nns[b]->type.isDumbbell()) {
                        eb_FeX += 0.70;
                    } else {
                        if (_1nns[b]->type._type == LatticeTypes::Cu) {
                            ecomp_FeX += -0.21;
                        } else if (_1nns[b]->type._type == LatticeTypes::Ni) {
                            ecomp_FeX += -0.32;
                        } else if (_1nns[b]->type._type == LatticeTypes::Mn) {
                            ecomp_FeX += -0.30;
                        }
                    }
                    break;
                case LatticeTypes::Mn:
                    if (_1nns[b]->type.isDumbbell()) {
                        eb_FeX += 0.70;
                    } else {
                        if (_1nns[b]->type._type == LatticeTypes::Cu) {
                            ecomp_FeX += 0.28;
                        } else if (_1nns[b]->type._type == LatticeTypes::Ni) {
                            ecomp_FeX += 0.29;
                        } else if (_1nns[b]->type._type == LatticeTypes::Mn) {
                            ecomp_FeX += 0.28;
                        }
                    }
                    break;
            }
        }
    }
    // for 2nn neighbour lattices.
    for (int b = 0; b < Itl::TRANS_DIRS_BITS_SIZE; b++) {
        if (((_2nn_status >> b) & 0x01) && _2nns[b]->type.isDumbbell()) {
            eb_FeX += 0.48;
        }
    }
    return ecomp_FeX + eb_FeX / 2;;
}
