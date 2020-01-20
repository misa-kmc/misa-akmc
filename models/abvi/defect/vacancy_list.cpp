//
// Created by genshen on 2018-12-18.
//

#include <cassert>
#include "utils/macros.h"
#include "vacancy_list.h"

_type_neighbour_status Vacancy::availTranDirs(_type_neighbour_status nei_status, Lattice **_1nn_lats) {
    // search all neighbour lattices, if the neighbour lattice is a destination that the source lattice can jump to
    // (it is available and it is atom),
    // then set the destination as available transition direction.
    _type_dirs_status atom_nei_status = 0;
    for (int b = 0; b < LatticesList::MAX_NEI_BITS; b++) {
#ifdef KMC_DEBUG_MODE
        if ((nei_status >> b) & 1) {
            // its neighbour lattice can not be dumbbell.
            bool is_db = _1nn_lats[b]->type.isDumbbell();
            assert(!is_db);
        }
#endif
        // the neighbour lattice is available.
        // and the neighbour lattice is atom
        if (((nei_status >> b) & 1) && _1nn_lats[b]->type.isAtom()) { // can trans
            atom_nei_status |= 1 << b;
        }
    }
    return atom_nei_status;
}

void Vacancy::beforeRatesUpdate(Lattice *list_1nn[LatticesList::MAX_1NN], _type_neighbour_status status_1nn) {
    // zero rates array
    Defect::beforeRatesUpdate(list_1nn, status_1nn);
    // set available transition dir.
    avail_trans_dir = availTranDirs(status_1nn, list_1nn);
}

void Vacancy::updateRates(Lattice &lattice, Lattice *list_1nn[LatticesList::MAX_1NN],
                          _type_neighbour_status status_1nn,
                          rateCallback callback) {
    for (unsigned char b = 0; b < LatticesList::MAX_NEI_BITS; b++) {
        if ((avail_trans_dir >> b) & 1) { // the neighbour lattice is available
            rates[b] = callback(list_1nn[b], list_1nn[b]->type._type, b);
        }
    }
}

void VacancyList::replace(const _type_lattice_id old_lat_id, const _type_lattice_id new_lat_id,
                          const LatListMeta *p_meta) {
#ifdef KMC_DEBUG_MODE
    assert(!p_meta->isGhostLat(old_lat_id));
#endif
    mp.erase(old_lat_id);
    // todo init vacancy
    if (!p_meta->isGhostLat(new_lat_id)) {
        mp.insert(std::make_pair(new_lat_id, Vacancy{}));
    }
}

void VacancyList::reindex(LatticesList *lats, const comm::Region<comm::_type_lattice_size> region) {
    for (int z = region.z_low; z < region.z_high; z++) {
        for (int y = region.y_low; y < region.y_high; y++) {
            for (int x = BCC_DBX * region.x_low; x < BCC_DBX * region.x_high; x++) {
                auto it = mp.find(lats->getId(x, y, z));
                Lattice &lattice = lats->getLat(x, y, z);
                if (lattice.type.isVacancy()) {
                    if (it == mp.end()) {
                        mp.insert(std::make_pair(lats->getId(x, y, z), Vacancy{}));
                    }
                } else {
                    if (it != mp.end()) {
                        mp.erase(it);
                    }
                }
            }
        }
    }
}
