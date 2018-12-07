//
// Created by zhaorunchun on 2018-12-06.
//

#include <vector>
#include <algorithm>
#include <map>
#include "atom_list.h"
#include "type_define.h"

using namespace std;

/*!
 * @brief 随机一些原子，包括坐标，类型，如果是间隙，需要取向信息。
 */
void AtomList::init() {

}


bool AtomList::get1nn(_type_atom_cord x, _type_atom_cord y, _type_atom_cord z, vector<_type_atom_pair> &Onenn_list) {
    Onenn_list.clear();
    for (int i = 0; i < 8; i++) {

        _type_atom_cord ix = x + AtomList::Onenn_x[i];
        _type_atom_cord iy = y + AtomList::Onenn_y[i];
        _type_atom_cord iz = z + AtomList::Onenn_z[i];

        Onenn_list.emplace_back(make_pair(ix, make_pair(iy, iz)));
    }
    return true;
}

bool AtomList::get2nn(_type_atom_cord x, _type_atom_cord y, _type_atom_cord z, vector<_type_atom_pair> &Twonn_list) {
    Twonn_list.clear();
    for (int i = 0; i < 6; i++) {

        _type_atom_cord ix = x + AtomList::Twonn_x[i];
        _type_atom_cord iy = y + AtomList::Twonn_y[i];
        _type_atom_cord iz = z + AtomList::Twonn_z[i];

        Twonn_list.emplace_back(make_pair(ix, make_pair(iy, iz)));
    }
    return true;
}

_type_atom_cord AtomList::getId(_type_atom_cord x, _type_atom_cord y, _type_atom_cord z) {
    return x * MAX_Y + y + z * MAX_X * MAX_Z;
}

int ItlList::getItlnum(_type_atom_cord id) {
    return AtomList::mp[id];
}
