//
// Created by genshen on 2019-03-08.
//

#include <cassert>
#include "counter.h"
#include "type_define.h"

int counter::getAtomCount(const LatticeTypes::lat_type tp) {
#ifdef DEBUG_MODE
    assert(LatticeTypes{tp}.isAtom() || LatticeTypes{tp}.isVacancy());
#endif
    if (LatticeTypes{tp}.isVacancy()) {
        return data[tp];
    }
    int count = 0;
    LatticeTypes::lat_type high, low;
    for (const auto pair :data) {
        high = LatticeTypes{pair.first}.getHighEnd();
        low = LatticeTypes{pair.first}.getLowEnd();
        if (high == tp) {
            count += pair.second;
        }
        if (low == tp) {
            count += pair.second;
        }
    }
    return count;
}

counter counter::newCounter(LatticesList *p_list) {
    counter c;
    for (_type_lattice_coord z = 0; z < p_list->meta.box_z; z++) {
        for (_type_lattice_coord y = 0; y < p_list->meta.box_y; y++) {
            // note: x is already doubled.
            for (_type_lattice_coord x = 0; x < p_list->meta.box_x; x++) {
                Lattice &lat = p_list->getLat(p_list->meta.ghost_x + x,
                                              p_list->meta.ghost_y + y,
                                              p_list->meta.ghost_z + z);
                c.add(lat.type._type);
            }
        }
    }
    return c;
}
