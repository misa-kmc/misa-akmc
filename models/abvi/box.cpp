//
// Created by genshen on 2018-12-09.
//

#include "box.h"
#include "lattice/period_lattice_list.h"

Box *BoxBuilder::build() {
    Box *box = new Box(); // todo remember to delete
    // create box: new array LatticesList, and set ItlList and VacancyList.
    box->createBox(box_x, box_y, box_z, ghost_x, ghost_y, ghost_z);
    return box;
}

Box::Box() : lattice_list(nullptr), va_list(nullptr), itl_list(nullptr) {}

Box::~Box() {
    delete lattice_list; // todo non-virtual destructor
    delete va_list;
    delete itl_list;
}

void Box::createBox(const _type_box_size box_x, const _type_box_size box_y, const _type_box_size box_z,
                    const _type_box_size ghost_x, const _type_box_size ghost_y, const _type_box_size ghost_z) {
    // todo other type of boundary.
    // new Lattice array and set lattice id.
    lattice_list = new PeriodLatticeList(box_x, box_y, box_z, ghost_x, ghost_y, ghost_z);
    va_list = new VacancyList();
    itl_list = new ItlList();
}
