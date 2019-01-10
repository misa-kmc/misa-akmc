//
// Created by zhaorunchu on 2018-12-06.
//

#ifndef MISA_KMC_LATTICES_LIST_H
#define MISA_KMC_LATTICES_LIST_H

#include <map>
#include <vector>
#include "type_define.h"
#include "lattice.h"

class LatticesList {
public:
    /*!
     * \brief initialize the lattice_lists array(allocate memory) with box size in x,y,z direction.
     * besides, the lattice id will be set in this constructor.
     * \param box_x box size in x direction.
     * \param box_y box size in y direction.
     * \param box_z box size in z direction.
     */
    LatticesList(_type_box_size box_x, _type_box_size box_y, _type_box_size box_z);

    ~LatticesList();

    /*!
     * \brief generate the lattices type, and direction if a lattice is inter lattice.
     * \param ratio the ratio of mixed alloy to except.
     * \param alloy_types the length of array @param ratio
     * \param va_rate the rate of vacancy
     */
    void randomInit(int ratio[], int alloy_types, double va_rate);

    // typedef of iteration of all lattices.
    typedef const std::function<bool(const _type_lattice_coord x,
                                     const _type_lattice_coord y,
                                     const _type_lattice_coord z,
                                     Lattice &lattice)> func_lattices_callback;

    /**
     * \brief iterate all lattice in this list, each lattice will be passed to callback function.
     * if the callback return false, iteration will break.
     */
    void forAllLattices(func_lattices_callback callback);

    /*!
     * \brief get all lattice near 1nn
     * \param x,y,z the coordinate of the lattice point.
     * \param _1nn_list a array to store all pointers of Lattices in the distance of 1nn.
     * \note note that the coordinate specified by [x,y,z] must be in the lattice box, or "index out of bounds" may happen.
     * \return the count of 1nn list.
     */
    int get1nn(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z, Lattice *_1nn_list[8]);

    /*!
     * \brief get all lattice near 2nn
     * \param x,y,z the coordinate of the lattice point.
     * \param _2nn_list a array to store all pointers of Lattices in the distance of 2nn.
     * \note note that the coordinate specified by [x,y,z] must be in the lattice box, or "index out of bounds" may happen.
     * \return the count in 2nn list.
     */
    int get2nn(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z, Lattice *_2nn_list[6]);

    /*!
     * \brief calculate the corresponding number of coordinate
     * \return Id
     */
    inline _type_lattice_id getId(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z) {
        return x + y * size_x + z * size_x * size_y; // todo return from Lattice object.
    }

    /**
     * \return the max lattice id of all lattices.
     */
    inline _type_lattice_id maxId() {
        return _max_id;
    }

    /**
     * \brief get Lattice object by lattice id
     *
     * @note in the implementations, we does not Guarantee the lattices array boundary.
     * If the lattice specified the @param id is out of box, your program may crash.
     *
     * \param id the given lattice id.
     * \return the reference of the matched lattice.
     */
    Lattice &getLatById(_type_lattice_id id);

private:
    /*!
     * \brief the size of lattice lists array in each dimension.
     * \note the size_x is two times then real box size due to BCC structure.
     * size_y and size_y is the same as the simulation real box size.
     */
    const _type_lattice_coord size_x, size_y, size_z;

    // the max lattice id in box.
    const _type_lattice_id _max_id;

    /*!
     * \brief the 3d array of all lattices.
     * the first dimension of this array represent x index of lattice in box,
     * then second and third represent y and z index.
     *
     * \note if the box size is [b_x, b_y, b_z], then the array size will be [2*b_x, b_y, b_z].
     * the size of array in first dimension is two times then the box size in x direction due to BCC structure.
     */
    Lattice ***_lattices = nullptr;
};


#endif //MISA_KMC_LATTICES_LIST_H
