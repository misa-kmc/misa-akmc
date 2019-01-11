//
// Created by genshen on 2019-01-10.
//

#ifndef MISA_KMC_PURE_LATTICE_LIST_H
#define MISA_KMC_PURE_LATTICE_LIST_H


#include "lattices_list.h"

/**
 * lattice list for normal boundary condition.
 */
class PureLatticeList : public LatticesList {
public:
    /**
     * \brief create the lattice list of non-boundary with box size: box_x*box_y*box_z.
     * \param box_x box size/lattice count in x direction.
     * \param box_y box size/lattice count in y direction.
     * \param box_z box size/lattice count in z direction.
     */
    PureLatticeList(_type_box_size box_x, _type_box_size box_y, _type_box_size box_z);

    /**
     * \brief get 1nn lattices of a lattice specified by \param x,y,z
     * \see declaration in in base class for mode details.
     * \param x,y,z coordinate of source lattice
     * \param _1nn_list array to save 1nn lattices
     * \return the count of 1nn lattice.
     */
    int get1nn(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z, Lattice *_1nn_list[8]) override;

    /**
     * \brief get 2nn lattices of a lattice specified by \param x,y,z
     * \see declaration in in base class for mode details.
     * \param x,y,z coordinate of source lattice
     * \param _2nn_list array to save 1nn lattices
     * \return the count of 2nn lattice.
     */
    int get2nn(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z, Lattice *_2nn_list[6]) override;
};


#endif //MISA_KMC_PURE_LATTICE_LIST_H
