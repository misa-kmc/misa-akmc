//
// Created by genshen on 2019-01-10.
//

#include "normal_lattice_list.h"
#include "../utils/macros.h"

NormalLatticeList::NormalLatticeList(const LatListMeta meta) : LatticesList(meta) {}

_type_neighbour_status NormalLatticeList::get1nnStatus(_type_lattice_coord x, _type_lattice_coord y,
                                                       _type_lattice_coord z) {
  return LatticesList::get1nnBoundaryStatus(x, y, z);
}

_type_neighbour_status NormalLatticeList::get2nnStatus(_type_lattice_coord x, _type_lattice_coord y,
                                                       _type_lattice_coord z) {
  return LatticesList::get2nnBoundaryStatus(x, y, z);
}

int NormalLatticeList::get1nn(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z,
                              Lattice *_1nn_list[MAX_1NN]) {
  // In our implementation, if x is even,then its 1nn will be ([x-1,x+1],
  // [y-1, y], [z-1,z]), if x is odd, then its 1nn will be ([x-1,x+1], [y,
  // y+1], [z,z+1]).
  _type_lattice_coord _1nn_index_x[MAX_1NN], _1nn_index_y[MAX_1NN], _1nn_index_z[MAX_1NN];

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
  _type_neighbour_status status = get1nnStatus(x, y, z);
  // save data into array
  int _count = 0;
  for (int i = 0; i < MAX_1NN; i++) {
    if (status & (0x01 << i)) {
      _1nn_list[i] = &_lattices[_1nn_index_z[i]][_1nn_index_y[i]][_1nn_index_x[i]];
      _count++;
    }
  }
  // calculate the size of 1nn_list.
  return _count;
}

int NormalLatticeList::get2nn(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z,
                              Lattice *_2nn_list[6]) {
  static const int _2nn_offset_x[] = {-2, 0, 0, 0, 0, 2};
  static const int _2nn_offset_y[] = {0, -1, 0, 0, 1, 0};
  static const int _2nn_offset_z[] = {0, 0, -1, 1, 0, 0};
  int _count = 0;
  if (x >= 2) {
    _2nn_list[0] = &_lattices[z + _2nn_offset_z[0]][y + _2nn_offset_y[0]][x + _2nn_offset_x[0]];
    _count++;
  }
  if (y != 0) {
    _2nn_list[1] = &_lattices[z + _2nn_offset_z[1]][y + _2nn_offset_y[1]][x + _2nn_offset_x[1]];
    _count++;
  }
  if (z != 0) {
    _2nn_list[2] = &_lattices[z + _2nn_offset_z[2]][y + _2nn_offset_y[2]][x + _2nn_offset_x[2]];
    _count++;
  }
  if (z + 1 != meta.size_z) {
    _2nn_list[3] = &_lattices[z + _2nn_offset_z[3]][y + _2nn_offset_y[3]][x + _2nn_offset_x[3]];
    _count++;
  }
  if (y + 1 != meta.size_y) {
    _2nn_list[4] = &_lattices[z + _2nn_offset_z[4]][y + _2nn_offset_y[4]][x + _2nn_offset_x[4]];
    _count++;
  }
  if (x + 2 < meta.size_x) {
    _2nn_list[5] = &_lattices[z + _2nn_offset_z[5]][y + _2nn_offset_y[5]][x + _2nn_offset_x[5]];
    _count++;
  }
  return _count;
}
