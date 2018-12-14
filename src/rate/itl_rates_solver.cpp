//
// Created by genshen on 2019-01-19.
//

#include <rate/bonds/bonds_counter.h>
#include "macros.h"
#include "itl_rates_solver.h"

ItlRatesSolver::ItlRatesSolver(Box &box, const double v, const double T) : RatesSolver(box) {
    // todo save T,v
}

_type_rate ItlRatesSolver::rate(const Lattice &source_lattice, const Lattice &target_lattice,
                                const LatticeTypes::lat_type trans_atom,
                                const _type_dir_id _1nn_offset) {
#ifdef EAM_POT_ENABLED
    return 0; // todo eam
#else
    // bonds energy of src lattice contributed by its 1nn/2nn neighbour lattice.
    _type_pair_ia e_src = BondsCounter::count(box.lattice_list, source_lattice.getId(), LatticeTypes{trans_atom});
    // bonds energy of des lattice contributed by its 1nn/2nn neighbour lattice.
    _type_pair_ia e_des = BondsCounter::count(box.lattice_list, _1nn_offset, target_lattice.type);
//    return arrhenius(v,t,);
    return 0;
#endif
}

double ItlRatesSolver::Edumb() {
    // used in each inter loop.
    Lattice *_1nn_list[LatticesList::MAX_1NN];
    Lattice *_2nn_list[LatticesList::MAX_2NN];

    double edumb = 0;

    for (auto const &itl_pair : box.itl_list->mp) {
//        itl_pair.second;
//        _type_lattice_coord xd, yd, zd;
        const _type_lattice_id &id = itl_pair.first;

        box.lattice_list->get1nn(id, _1nn_list);
        box.lattice_list->get2nn(id, _2nn_list);

        double emix = 0;
        double etens = 0;
        double ecomp = 0;
        double eb = 0;

        switch (box.lattice_list->getLat(id).type._type) {
            case LatticeTypes::FeCu:
                emix = -0.46;
                ecomp = FeXcomp(xd, yd, zd);
                break;

            case LatticeTypes::CuCu:
                emix = -0.36;
                Edumb_for();
                break;
            case LatticeTypes::FeNi:
                emix = -0.19;
                ecomp = FeXcomp(xd, yd, zd);
                break;

            case LatticeTypes::CuNi:
                emix = -0.5;
                Edumb_for();
                break;

            case LatticeTypes::NiNi:
                emix = -0.3;
                Edumb_for();
                break;

            case LatticeTypes::FeMn:
                emix = 0.39;
                ecomp = FeXcomp(xd, yd, zd);
                break;
            case LatticeTypes::CuMn:
                emix = -0.11;
                Edumb_for();
                break;
            case LatticeTypes::NiMn:
                emix = 0.08;
                Edumb_for();
                break;
            case LatticeTypes::MnMn:
                emix = 0.41;
                Edumb_for();
                break;
            case LatticeTypes::FeFe:
                etens = 0;
                for (it = _1nn_list.begin(); it != _1nn_list.end(); it++) {
                    LatticeTypes StateDB = _atoms[it->first][it->second.first][it->second.second].atom_type;
                    if (StateDB != ItlList::dir1[itl.dir_num] && StateDB != ItlList::dir2[itl.dir_num]
                        && StateDB != ItlList::dir3[itl.dir_num] && StateDB != ItlList::dir4[itl.dir_num]) {
                        if (StateDB == 3) etens += 0.13;
                        if (StateDB == 7) etens += 0.02;
                        if (StateDB == 12) etens += -0.36;
                        if (StateDB != 3 && StateDB != 7
                            && StateDB != 12 && StateDB != 1 && StateDB != 0)
                            eb += 0.70;

                    }
                    if (StateDB == ItlList::dir1[itl.dir_num] || StateDB == ItlList::dir2[itl.dir_num]
                        || StateDB == ItlList::dir3[itl.dir_num] || StateDB == ItlList::dir4[itl.dir_num]) {
                        if (StateDB == 3) ecomp += -0.13;
                        if (StateDB == 7) ecomp += 0.15;
                        if (StateDB == 12) ecomp += 0.02;
                        if (StateDB != 3 && StateDB != 7
                            && StateDB != 12 && StateDB != 1 && StateDB != 0)
                            eb += 0.70;
                    }
                }
                for (it = _2nn_list.begin(); it != _2nn_list.end(); it++) {
                    StateDB = _atoms[it->first][it->second.first][it->second.second].atom_type;
                    if (StateDB != 3 && StateDB != 7
                        && StateDB != 12 && StateDB != 1 && StateDB != 0)
                        eb += 0.48;
                }
                break;
            default:
                emix = 0;
                etens = 0;
                ecomp = 0;
                eb = 0;
                break;
        }
        edumb += emix + etens + ecomp + eb / 2;
    }
    return edumb;
}
