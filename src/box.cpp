//
// Created by genshen on 2018-12-09.
//

#include "box.h"
#include "lattice/normal_lattice_list.h"

Box *BoxBuilder::build() {
    Box *box = new Box(size_x, size_y, size_z, 0, 0); // todo remember to delete
    box->createBox(); // create box: new array LatticesList, and set ItlList and VacancyList.
    return box;
}

Box::Box(_type_box_size size_x, _type_box_size size_y, _type_box_size size_z, double v, double T)
        : size_x(size_x), size_y(size_y), size_z(size_z),
          v(v), T(T),
          lattice_list(nullptr), va_list(nullptr), itl_list(nullptr) {
}

void Box::createBox() {
    // todo other type of boundary.
    lattice_list = new NormalLatticeList(size_x, size_y, size_z); // new Lattice array and set lattice id.
}
