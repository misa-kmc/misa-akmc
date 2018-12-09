//
// Created by zhaorunchu on 2018-12-06.
//

#ifndef MISA_KMC_ATOM_LIST_H
#define MISA_KMC_ATOM_LIST_H

#include <map>
#include <vector>
#include "type_define.h"
#include "atom.h"

class AtomList {
public:
    /*!
     * \brief initialize the atoms array(allocate memory) with box size in x,y,z direction.
     * \param box_x box size in x direction.
     * \param box_y box size in y direction.
     * \param box_z box size in z direction.
     */
    AtomList(_type_box_size box_x, _type_box_size box_y, _type_box_size box_z);

    ~AtomList();
//    vector<int> Onenn_list, Twonn_list;

    //find Itl id in Atom.
    static std::map<_type_atom_cord, int> mp;


    /*!
     * \brief 随机一些原子，包括坐标，类型，如果是间隙，需要取向信息。
     */
    void init();

    /*!
     * \brief get all lattice near 1nn
     * \param _1nn_list a vector to store all atoms in the distance of 1nn
     * \return the count of 1nn list.
     */
    int get1nn(_type_atom_cord x, _type_atom_cord y, _type_atom_cord z, std::vector<_type_atom_pair> &_1nn_list);

    /*!
     * \brief get all lattice near 1nn
     * \param _2nn_list a vector to store all atoms in the distance of 1nn
     * \return the count of 2nn list.
     */
    int get2nn(_type_atom_cord x, _type_atom_cord y, _type_atom_cord z, std::vector<_type_atom_pair> &_2nn_list);

    /*!
     * \brief calculate the corresponding number of coordinate
     * \return Id
     */
    _type_atom_id getId(_type_atom_cord x, _type_atom_cord y, _type_atom_cord z);

private:
    /*!
     * \brief the size of atom array in each dimension.
     * \note the size_x is two times then real box size due to BCC structure.
     * size_y and size_y is the same as the simulation real box size.
     */
    const _type_atom_cord size_x, size_y, size_z;

    /*!
     * \brief the 3d array of all Atoms.
     * the first dimension of this array represent x index of atoms in box,
     * then second and third represent y and z index.
     *
     * \note if the box size is [b_x, b_y, b_z], then the array size will be [2*b_x, b_y, b_z].
     * the size of array in first dimension is two times then the box size in x direction due to BCC structure.
     */
    Atoms ***_atoms = nullptr;
};

class ItlList {
public:
    //a map to correspond the id and the Itl sequence number.
    std::map<int, int> mp;

    /*!
     * \brief to find the sequence number in class Ilt
     *        which include the extra information about Itl type atoms
     * \param id use the AtomList getId
     * \return the sequence number in in Itl
     */
    int getItlnum(_type_atom_cord id);
};

#endif //MISA_KMC_ATOM_LIST_H
