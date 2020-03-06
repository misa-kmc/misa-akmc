//
// Created by genshen on 2019-01-10.
//

#ifndef MISA_KMC_PERIODIC_LATTICE_LIST_H
#define MISA_KMC_PERIODIC_LATTICE_LIST_H

#include "lattices_list.h"

/**
 * lattice list for periodic boundary condition.
 */
class PeriodLatticeList : public LatticesList {
public:
  /**
   * \brief create the lattice list of period-boundary with box metadata
   * parameters. (including global and local box size, ghost size and global
   * base coordinate).
   */
  explicit PeriodLatticeList(const LatListMeta meta);

  _type_neighbour_status get1nnStatus(_type_lattice_coord x,
                                      _type_lattice_coord y,
                                      _type_lattice_coord z) override;

  _type_neighbour_status get2nnStatus(_type_lattice_coord x,
                                      _type_lattice_coord y,
                                      _type_lattice_coord z) override;

  /**
   * \brief get all lattices near 1nn
   * \param x x coordinate, doubled
   * \param y y coordinate
   * \param z z coordinate
   * \param _1nn_list a array to store all pointers of Lattices in the
   * distance of 1nn. \return the lattice pointers count in 1nn list
   */
  int get1nn(_type_lattice_coord x, _type_lattice_coord y,
             _type_lattice_coord z, Lattice *_1nn_list[MAX_1NN]) override;

  /**
   * \brief similar as above one, but it is for 2nn neighbor lattice.
   * \param x x coordinate, doubled
   * \param y y coordinate
   * \param z z coordinate
   * \param _1nn_list a array to store all pointers of Lattices in the
   * distance of 2nn. \return the lattice pointers count in 2nn list
   */
  int get2nn(_type_lattice_coord x, _type_lattice_coord y,
             _type_lattice_coord z, Lattice *_2nn_list[6]) override;
};

#endif // MISA_KMC_PERIODIC_LATTICE_LIST_H
