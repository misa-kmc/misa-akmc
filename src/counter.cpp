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
