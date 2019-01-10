//
// Created by genshen on 2018-12-12.
//

#ifndef MISA_KMC_LATTICE_TYPES_H
#define MISA_KMC_LATTICE_TYPES_H


#include <cstdint>

struct LatticeTypes {
    typedef enum {
        V = 0, Fe = 0x0001, Cu = 0x0002, Ni = 0x0004, Mn = 0x0008,
        FeFe = 0x0101, FeCu = 0x0102, FeNi = 0x0104, FeMn = 0x0108,
        CuCu = 0x0202, CuNi = 0x0204, CuMn = 0x0208,
        NiNi = 0x0404, NiMn = 0x0408,
        MnMn = 0x0808,
    } lat_type;

    lat_type _type;

    // initialize @member _type
    explicit LatticeTypes(lat_type tp) : _type(tp) {}
    // default value of _type is V
    explicit LatticeTypes() : _type(V) {}

    const static uint16_t c = 4;
    const static uint16_t dumbbell_critical_point = 1 << (c - 1); // 0x0008 (max single atom enum)

    bool isDumbbell() {
        return _type > dumbbell_critical_point;
    }

    bool isAtom() {
        return (_type <= dumbbell_critical_point) && (_type != V);
    }
};


#endif //MISA_KMC_LATTICE_TYPES_H
