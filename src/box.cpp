//
// Created by genshen on 2018-12-09.
//

#include "box.h"

Box *BoxBuilder::build() {
    Box *box = new Box(size_x, size_y, size_z); // todo remember to delete
    box->createBox(); // create box: new array AtomList, and set ItlList and VacancyList.
    return box;
}

Box::Box(_type_box_size size_x, _type_box_size size_y, _type_box_size size_z)
        : atom_list(nullptr), va_list(nullptr), itl_list(nullptr),
          size_x(size_x), size_y(size_y), size_z(size_z) {
}

void Box::createBox() {
    atom_list = new AtomList(size_x, size_y, size_z); // new atom array and set lattice id.
}
