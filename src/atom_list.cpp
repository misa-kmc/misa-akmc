//
// Created by zhaorunchun on 2018-12-06.
// Update by genshen on 2018-12-09.
//

#include <vector>
#include <algorithm>
#include <map>
#include "atom_list.h"
#include "type_define.h"
#include "macros.h"

AtomList::AtomList(_type_box_size box_x, _type_box_size box_y, _type_box_size box_z)
        : size_x(2 * box_x), size_y(box_y), size_z(box_z) {
    _atoms = new Atoms **[size_z];
    for (_type_atom_size z = 0; z < size_z; z++) {
        _atoms[z] = new Atoms *[size_y];
        for (_type_atom_size y = 0; y < size_y; y++) {
            _atoms[z][y] = new Atoms[size_x];
        }
    }
    // set id
    _type_atom_id id = 0;
    for (_type_atom_size z = 0; z < size_z; z++) {
        for (_type_atom_size y = 0; y < size_y; y++) {
            for (_type_atom_size x = 0; x < size_x; x++) {
                _atoms[z][y][x].id = id++;
            }
        }
    }
}

AtomList::~AtomList() {
    for (_type_atom_size z = 0; z < size_z; z++) {
        for (_type_atom_size y = 0; y < size_y; y++) {
            delete[] _atoms[z][y];
        }
        delete[] _atoms[z];
    }
    delete[] _atoms;
}

void AtomList::init() {
}

int
AtomList::get1nn(_type_atom_cord x, _type_atom_cord y, _type_atom_cord z, std::vector<_type_atom_pair> &_1nn_list) {
    _1nn_list.clear(); // todo use array.
    // In our implementation, if x is even,then its 1nn will be ([x-1,x+1], [y-1, y], [z-1,z]),
    // if x is odd, then its 1nn will be ([x-1,x+1], [y, y+1], [z,z+1]).
    // todo periodic boundary?
    _type_atom_cord _1nn_index_x[8], _1nn_index_y[8], _1nn_index_z[8];

    // compute 1nn index in array in x direction.
    // it can be overflow if x == 0.
    ASSIGN_ARRAY_8(_1nn_index_x, x - 1, x - 1, x - 1, x - 1, x + 1, x + 1, x + 1, x + 1);
    // 1nn index in y direction and z direction
    if (x % 2 == 0) {
        ASSIGN_ARRAY_8(_1nn_index_y, y - 1, y - 1, y, y, y - 1, y - 1, y, y)
        ASSIGN_ARRAY_8(_1nn_index_z, z - 1, z, z - 1, z, z - 1, z, z - 1, z)
    } else {
        ASSIGN_ARRAY_8(_1nn_index_y, y, y, y + 1, y + 1, y, y, y + 1, y + 1)
        ASSIGN_ARRAY_8(_1nn_index_z, z, z + 1, z, z + 1, z, z + 1, z, z + 1)
    }
    // compute returned index flag(it is not always 8 element).
    unsigned char flag = 0xFF; // binary 1 for keeping, 0 for abandon
    if (x == 0) {
        flag &= 0xF0; // invalid first 4 index.
    }
    if (x + 1 == size_x) {
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
        if (y + 1 == size_y) {
            flag &= 0x33; // 0b00110011 = 0x033
        }
        if (z + 1 == size_z) {
            flag &= 0x55; // 0b01010101 = 0x55
        }
    }
    // save data into array
    int _count = 0;
    for (int i = 0; i < 8; i++) {
        if ((flag >> i) & 0x01) {
            _count++;
            _1nn_list.emplace_back(std::make_pair(_1nn_index_x[i], std::make_pair(_1nn_index_y[i], _1nn_index_z[i])));
        }
    }
    // calculate the size of 1nn_list.
    return _count;
}

int
AtomList::get2nn(_type_atom_cord x, _type_atom_cord y, _type_atom_cord z, std::vector<_type_atom_pair> &_2nn_list) {
    _2nn_list.clear();
    static const int _2nn_offset_x[] = {-2, 0, 0, 0, 0, 2};
    static const int _2nn_offset_y[] = {0, -1, 0, 0, 1, 0};
    static const int _2nn_offset_z[] = {0, 0, -1, 1, 0, 0};
    int _count = 0;
    if (x >= 2) {
        // todo make it an inline func.
        _count++;
        _2nn_list.emplace_back(make_pair(x + _2nn_offset_x[0],
                                         std::make_pair(y + _2nn_offset_y[0], z + _2nn_offset_z[0])));
    }
    if (y != 0) {
        _count++;
        _2nn_list.emplace_back(make_pair(x + _2nn_offset_x[1],
                                         std::make_pair(y + _2nn_offset_y[1], z + _2nn_offset_z[1])));
    }
    if (z != 0) {
        _count++;
        _2nn_list.emplace_back(make_pair(x + _2nn_offset_x[2],
                                         std::make_pair(y + _2nn_offset_y[2], z + _2nn_offset_z[2])));
    }
    if (z + 1 != size_z) {
        _count++;
        _2nn_list.emplace_back(make_pair(x + _2nn_offset_x[3],
                                         std::make_pair(y + _2nn_offset_y[3], z + _2nn_offset_z[3])));
    }
    if (y + 1 != size_y) {
        _count++;
        _2nn_list.emplace_back(make_pair(x + _2nn_offset_x[4],
                                         std::make_pair(y + _2nn_offset_y[4], z + _2nn_offset_z[4])));
    }
    if (x + 2 >= size_x) {
        _count++;
        _2nn_list.emplace_back(make_pair(x + _2nn_offset_x[5],
                                         std::make_pair(y + _2nn_offset_y[5], z + _2nn_offset_z[5])));
    }
    return _count;
}

_type_atom_id AtomList::getId(_type_atom_cord x, _type_atom_cord y, _type_atom_cord z) {
    return x + y * size_x + z * size_x * size_y; // todo return from atom object.
}

int ItlList::getItlnum(_type_atom_cord id) {
    return AtomList::mp[id];
}
