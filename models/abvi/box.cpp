//
// Created by genshen on 2018-12-09.
//

#include "box.h"
#include "lattice/period_lattice_list.h"

Box *BoxBuilder::build() {
  Box *box = new Box(); // todo remember to delete
  // create box: new array LatticesList, and set ItlList and VacancyList.
  LatListMeta lattice_meta{box_x,        box_y,        box_z,        ghost_x,       ghost_y,       ghost_z,
                           global_box_x, global_box_y, global_box_z, global_base_x, global_base_y, global_base_z};
  box->createBox(lattice_meta);
  return box;
}

Box::Box() : lattice_list(nullptr), va_list(nullptr), itl_list(nullptr) {}

Box::~Box() {
  delete lattice_list;
  delete va_list;
  delete itl_list;
}

void Box::createBox(const LatListMeta lattice_meta) {
  // todo other type of boundary.
  // new Lattice array and set lattice id.

  lattice_list = new PeriodLatticeList(lattice_meta);
  va_list = new VacancyList();
  itl_list = new ItlList();
}
