//
// Created by zhaorunchun on 2018-12-06.
// Update by genshen on 2018-12-09.
//

#include <comm/domain/region.hpp>
#include "lattices_list.h"
#include "utils/macros.h"
#include "type_define.h"

LatticesList::LatticesList(const _type_box_size box_x, const _type_box_size box_y, const _type_box_size box_z,
                           const _type_box_size ghost_x, const _type_box_size ghost_y,
                           const _type_box_size ghost_z)
        : meta(box_x, box_y, box_z, ghost_x, ghost_y, ghost_z) {
    _lattices = new Lattice **[meta.size_z];
    for (_type_lattice_size z = 0; z < meta.size_z; z++) {
        _lattices[z] = new Lattice *[meta.size_y];
        for (_type_lattice_size y = 0; y < meta.size_y; y++) {
            _lattices[z][y] = new Lattice[meta.size_x];
        }
    }
    // set id (skip ghost area id setting)
    _type_lattice_id id = 0;
    comm::Region<_type_lattice_size> box_region{
            BCC_DBX * ghost_x, ghost_y, ghost_z,
            BCC_DBX * ghost_x + BCC_DBX * box_x, ghost_y + box_y, ghost_z + box_z,
    }; // note: index at x dimension is doubled.
    for (_type_lattice_size z = box_region.z_low; z < box_region.z_high; z++) {
        for (_type_lattice_size y = box_region.y_low; y < box_region.y_high; y++) {
            for (_type_lattice_size x = box_region.x_low; x < box_region.x_high; x++) {
                _lattices[z][y][x].id = id++;
            }
        }
    }
}

LatticesList::~LatticesList() {
    for (_type_lattice_size z = 0; z < meta.size_z; z++) {
        for (_type_lattice_size y = 0; y < meta.size_y; y++) {
            delete[] _lattices[z][y];
        }
        delete[] _lattices[z];
    }
    delete[] _lattices;
}

void LatticesList::forAllLattices(const func_lattices_callback callback) {
    for (_type_lattice_size z = 0; z < meta.size_z; z++) {
        for (_type_lattice_size y = 0; y < meta.size_y; y++) {
            for (_type_lattice_size x = 0; x < meta.size_x; x++) {
                if (!callback(x, y, z, _lattices[z][y][x])) {
                    return;
                }
            }
        }
    }
}

_type_neighbour_status LatticesList::get1nnBoundaryStatus(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z) {
    _type_neighbour_status flag = 0xFF; // binary 1 for keeping, 0 for abandon
    if (x == 0) {
        flag &= 0xF0; // invalid first 4 index.
    }
    if (x + 1 == meta.size_x) {
        flag &= 0x0F; // invalid last 4 index.
    }
    if (x % 2 == 0) {
        if (y == 0) {
            flag &= 0xCC; // 0b11001100 = 0x0CC
        }
        if (z == 0) {
            flag &= 0xAA; // 0b10101010 = 0xAA
        }
    } else {
        if (y + 1 == meta.size_y) {
            flag &= 0x33; // 0b00110011 = 0x033
        }
        if (z + 1 == meta.size_z) {
            flag &= 0x55; // 0b01010101 = 0x55
        }
    }
    return flag;
}

_type_neighbour_status LatticesList::get2nnBoundaryStatus(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z) {
    _type_neighbour_status status = 0x3F; // 0b00111111
    // the order is from x to z
    // (increase x from lower to higher; if x is the same, increase y from lower to higher;
    // if y is the same, increase z from lower to higher.)
    if (x < 2) {
        status &= 0x3E; // 0b 0011 1110
    }
    if (y == 0) {
        status &= 0x3D; // 0b 0011 1101
    }
    if (z == 0) {
        status &= 0x3B; // 0b 0011 1011
    }
    if (z + 1 == meta.size_z) {
        status &= 0x37; // 0b 0011 0111
    }
    if (y + 1 == meta.size_y) {
        status &= 0x2F; // 0b 0010 1111
    }
    if (x + 2 >= meta.size_x) {
        status &= 0x1F; // 0b 0001 1111
    }
    return status;
}

Lattice &LatticesList::getLat(_type_lattice_id id) {
    _type_lattice_coord x = id % meta.size_x;
    id = id / meta.size_x;
    _type_lattice_coord y = id % meta.size_y;
    _type_lattice_coord z = id / meta.size_y;
    return _lattices[z][y][x];
}

Lattice *LatticesList::walk(_type_lattice_id id, const _type_lattice_offset offset_x,
                            const _type_lattice_offset offset_y, const _type_lattice_offset offset_z) {
    _type_lattice_coord sx = id % meta.size_x;
    id = id / meta.size_x;
    // use half lattice const coord.
    _type_lattice_coord sy = sx % 2 == 0 ? 2 * (id % meta.size_y) : 2 * (id % meta.size_y) + 1;
    _type_lattice_coord sz = sx % 2 == 0 ? 2 * (id / meta.size_y) : 2 * (id / meta.size_y) + 1;

    // add offset
    sx += offset_x;
    sy += offset_y;
    sz += offset_z;

    //if out of index
    if (sx >= meta.size_x || sy >= 2 * meta.size_y || sz >= 2 * meta.size_z) {
        return nullptr;
    }
    // if sx is even, sy and sz should also be even.
    if (sx % 2 == 0 && sy % 2 == 0 && sz % 2 == 0) {
        return &_lattices[sz / 2][sy / 2][sx];
    }
    // if sx is odd, sy and sz should also be odd.
    if (sx % 2 == 1 && sy % 2 == 1 && sz % 2 == 1) {
        return &_lattices[sz / 2][sy / 2][sx];
    }
    return nullptr;
}
