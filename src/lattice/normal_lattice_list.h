//
// Created by genshen on 2019-01-10.
//

#ifndef MISA_KMC_PURE_LATTICE_LIST_H
#define MISA_KMC_PURE_LATTICE_LIST_H

#include "lattices_list.h"

/**
 * lattice list for normal boundary condition.
 */
class NormalLatticeList : public LatticesList {
public:
  /**
   * \brief create the lattice list of non-boundary with box size:
   * box_x*box_y*box_z. In this constructor, the ghost area is not specified,
   * which is usually used in non-parallel mode. \param box_x box size/lattice
   * count in x direction. \param box_y box size/lattice count in y direction.
   * \param box_z box size/lattice count in z direction.
   * \deprecated this is for serial code.
   */
  NormalLatticeList(_type_box_size box_x, _type_box_size box_y, _type_box_size box_z)
      : NormalLatticeList(LatListMeta{box_x, box_y, box_z, 0, 0, 0}) {}

  /**
   * \brief create non-boundary lattice list with meta data
   *    (including global and local box size, ghost size and global base
   * coordinate).
   */
  NormalLatticeList(const LatListMeta meta);

  /**
   * \brief get bit status of 1nn neighbour lattice.
   * \see declaration in in base class for more details.
   *
   * If the source lattice specified by \param x,y,z is at a corner or box
   * boundary, some 1nn neighbour lattices may be nonexistent (normal boundary
   * condition), so the corresponding bits will be set to 0.
   *
   * \param x,y,z coordinate of source lattices
   * \return bits for status of 1nn neighbour lattices.
   */
  _type_neighbour_status get1nnStatus(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z) override;

  /**
   * \brief get bit status of 2nn neighbour lattice.
   * \see declaration in in base class for mode details.
   * similar to get1nnStatus, but this methods is for 2nn neighbour lattices.
   *
   * \param x,y,z coordinate of source lattices
   * \return  bits for status of 2nn neighbour lattices.
   */
  _type_neighbour_status get2nnStatus(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z) override;

  /**
   * \brief get 1nn lattices of a lattice specified by \param x,y,z
   * \see declaration in in base class for more details.
   * \param x,y,z coordinate of source lattice
   * \param _1nn_list array to save 1nn lattices
   * \return the lattice pointers count of 1nn lattice.
   */
  int get1nn(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z, Lattice *_1nn_list[MAX_1NN]) override;

  /**
   * \brief get 2nn lattices of a lattice specified by \param x,y,z
   * \see declaration in in base class for more details.
   * \param x,y,z coordinate of source lattice
   * \param _2nn_list array to save 1nn lattices
   * \return the count of 2nn lattice.
   */
  int get2nn(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z, Lattice *_2nn_list[6]) override;
};

#endif // MISA_KMC_PURE_LATTICE_LIST_H
